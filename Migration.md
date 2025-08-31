# Migration plan: Make render_script and render_api multi-backend friendly

This document describes a concrete, incremental migration to make the existing
`render_script` command buffer and `render_api` infrastructure ready for
future Metal and Vulkan backends while preserving current OpenGL behavior.

Goals and constraints
- Preserve all public APIs (no breaking changes to `render_script` signatures).
- Keep current OpenGL path working during migration (default behavior must be
  identical after each incremental change).
- Enable a clear path to efficient Vulkan/Metal implementations (pipelines,
  descriptor sets, encoders) with minimal future friction.
- Make changes incremental and testable; prefer small, reversible edits.

Checklist (requirements)
- Introduce an executor/encoder abstraction so command implementations stop
  calling `render_api` directly. (Phase 1)
- Keep `render_api` focused on resource creation & utility helpers.
- Add a minimal `pipeline` abstraction so shaders+state map to PSOs later.
- Provide reflection / uniform mapping guidance for name-based uniform API.
- Add tests/build checks to ensure behavior remains identical on GL.

High-level approach
- Add a runtime-pluggable `command_executor` interface in
  `mgl::platform::api`. Implement a default executor that forwards to the
  existing `render_api` static methods so behavior is unchanged.
- Replace direct calls to `mgl::platform::api::render_api::...` in command
  classes with calls to `command_executor::instance()->...` (mechanical refactor
  across command files). This is non-breaking because the default executor
  forwards to the GL path.
- Add a small `pipeline` abstraction (thin wrapper around `program_ref`) to
  converge on pipeline semantics; keep `enable_shader(...)` as a compatibility
  shim that resolves to a `bind_pipeline` command.
- Later add `command_encoder`/`command_buffer` interfaces that encode recorded
  commands into native backend command buffers for Vulkan/Metal.

Phased work (concrete)

Phase 0 — Rules (apply now)
- Do not change `render_script` public signatures.
- Commands must keep holding `*_ref` objects, not raw handles.
- Make changes incremental and compile-test after each small batch.

Phase 1 — Executor abstraction (POC, non-breaking) — implement first
Add:
- `mgl/platform/inc/public/mgl_platform/api/command_executor.hpp`
  - Pure-virtual interface `command_executor` listing methods used by commands:
    clear, set_viewport, set_view_matrix, set_projection_matrix,
    enable_state/disable_state, bind_texture, enable_program/disable_program,
    set_program_uniform (all overloads), set_blend_func/equation,
    clear_samplers, render_call(vertex/index/batch).
  - Singleton accessor: `command_executor::instance()` and setter.
- `mgl/platform/src/command_executor_default.cpp`
  - Default implementation that forwards every call to the existing
    `render_api::` static methods.

Edit (mechanical replacements):
- In command classes replace `mgl::platform::api::render_api::X(...)` with
  `mgl::platform::api::command_executor::instance().X(...)` or equivalent.
  Files to update in small batches:
  - `mgl/graphics/inc/mgl_graphics/commands/functions.hpp`
  - `mgl/graphics/inc/mgl_graphics/commands/draw.hpp`
  - `mgl/graphics/inc/mgl_graphics/commands/texture.hpp`
  - `mgl/graphics/inc/mgl_graphics/commands/shader.hpp`
  - `mgl/graphics/inc/mgl_graphics/commands/state.hpp`

Validation after Phase 1:
- Build (cmake configure + build) — should pass.
- Run a known example that uses `render_script` — visual output must match
  previous baseline.

Phase 2 — Pipeline abstraction (additive)
Add:
- `mgl/platform/inc/public/mgl_platform/api/pipeline.hpp`
  - `pipeline_desc` (vertex/fragment program refs, minimal blend/raster state)
  - `pipeline` class + `pipeline_ref` that for now wraps/contains `program_ref`.

Edits:
- Implement `bind_pipeline_command` (new command type) and make
  `render_script::enable_shader(...)` resolve/create a `pipeline_ref` and
  submit a `bind_pipeline` command (compat shim). The old `enable_shader` command
  can be kept but should call into executor->bind_pipeline.

Why: Vulkan/Metal map naturally to pipelines/PSOs. GL remains an adapter.

Phase 3 — Reflection & descriptor plan
- Add optional reflection metadata to `program`/`pipeline`: map uniform name ->
  type/binding/offset. Implemented initially for GL using introspection.
- Keep `set_shader_uniform(name, value)` commands but let the backend executor
  aggregate them into UBO/push-constant updates using reflection mapping.
- Add a light `descriptor_set` abstraction in `mgl::platform::api` for grouping
  textures/buffers. GL backend: simple emulation. Vulkan/Metal: real descriptor
  allocations.

Phase 4 — Command encoder / native recording (backend-specific)
- Add `mgl/platform/inc/public/mgl_platform/api/command_encoder.hpp` with a
  `command_encoder` interface: begin_pass, bind_pipeline, bind_descriptor_set,
  set_viewport, push_constants, draw, end_pass, finish. This will be used by
  Vulkan/Metal backends to record native command buffers.
- Implement a GL encoder that either forwards to `command_executor` or uses the
  `render_api` methods directly (compatibility).

Phase 5 — Tests, performance & optimizations
- Unit tests: record a `render_script` and assert it contains expected command
  objects and values.
- Smoke tests: compare render outputs before/after the migration.
- Optimization tasks later: uniform aggregation, descriptor caching, pipeline
  object caching.

File-level plan (what to create/edit now)
- Add files (Phase 1):
  - `mgl/platform/inc/public/mgl_platform/api/command_executor.hpp`
  - `mgl/platform/src/command_executor_default.cpp`
- Edit files (mechanical replacements):
  - `mgl/graphics/inc/mgl_graphics/commands/functions.hpp`
  - `mgl/graphics/inc/mgl_graphics/commands/draw.hpp`
  - `mgl/graphics/inc/mgl_graphics/commands/texture.hpp`
  - `mgl/graphics/inc/mgl_graphics/commands/shader.hpp`
  - `mgl/graphics/inc/mgl_graphics/commands/state.hpp`
- Add later (Phase 2+):
  - `mgl/platform/inc/public/mgl_platform/api/pipeline.hpp`
  - `mgl/platform/inc/public/mgl_platform/api/command_encoder.hpp`

Coding-agent prompt (use this to implement Phase 1 POC)
--------------------------------------------------
Goal: implement Phase 1 proof-of-concept so recorded commands call a
`command_executor` instead of `render_api` directly. The default executor must
forward to existing `render_api` behavior so everything keeps working.

Steps for the agent (apply changes on a feature branch named
`feature/record-commands-phase1` and open a PR when done):
1. Add header `mgl/platform/inc/public/mgl_platform/api/command_executor.hpp`.
   - Provide an interface `class command_executor` in namespace
     `mgl::platform::api` with the following pure virtual methods (signatures
     may mirror the corresponding `render_api` static wrappers):
     - clear(const glm::vec4&)
     - set_viewport(const glm::vec2&, const glm::vec2&)
     - set_view_matrix(const glm::mat4&)
     - set_projection_matrix(const glm::mat4&)
     - enable_state(int32_t) / disable_state(int32_t)
     - bind_texture(int32_t, const texture_ref&)
     - enable_program(const program_ref&) / disable_program()
     - set_program_uniform overloads matching `render_api`
     - set_blend_func/equation
     - clear_samplers(int, int)
     - render_call(vertex_buffer_ref, index_buffer_ref, int32_t count, int32_t offset, render_mode)
     - render_call(render_batch_ref)
   - Add `static command_executor& instance()` accessor and `static void set_instance(command_executor*)`
     (or use a `std::unique_ptr` install function). Keep API minimal and thread-agnostic.

2. Add implementation `mgl/platform/src/command_executor_default.cpp`.
   - Implement a `default_command_executor` that forwards each method to
     `render_api::...` equivalents.
   - Ensure the default instance is registered/installed during platform
     initialization (or provide an explicit install function called from the
     existing renderer init path).

3. Replace direct `render_api` calls inside the command classes to use the
   executor instead (mechanical edits). Do this in small batches and build
   after each batch:
   - Batch A (POC): `functions.hpp` -> replace clear/set_viewport/set_view/projection/set_blend_* /clear_samplers
   - Batch B: `draw.hpp` -> render_call usages
   - Batch C: `texture.hpp` -> bind_texture
   - Batch D: `state.hpp` -> enable_state/disable_state
   - Batch E: `shader.hpp` -> enable_program/disable_program and set_program_uniform usages

4. Build and test after each batch. On success, commit changes with a clear
   message for each batch (e.g., "commands: route function commands thru command_executor").

Validation steps the agent must run and include in the PR description:
- cmake configure + cmake build (or the project's existing build task). Include
  the build output summary and confirm success.
- Run a small smoke example that uses `render_script` (if repository contains an
  examples app, run it) and confirm no functional regressions. If automated
  rendering comparison is not available, manually verify the example runs.
- Add/modify a unit test that verifies `clear(glm::vec4(1.0f))` records the
  `clear_command` and that executing the recorded command calls through the
  executor (can be a small test that installs a test executor which records
  calls instead of forwarding to GL).

Notes and rationale for the agent
- Keep changes small and localized; use the default executor to preserve
  behavior.
- Do not change public `render_script` method signatures.
- Use the existing `mgl::platform::api` types (refs, enums) for parameters so
  command objects stay platform-agnostic.

Acceptance criteria for the PR
- All builds pass.
- A smoke example executes and renders with behavior identical to the pre-PR
  baseline.
- The PR includes the new `command_executor` interface and default impl, and
  at least `functions.hpp` commands pass through the executor (POC).

Follow-ups (after Phase 1)
- Migrate remaining command files to call `command_executor` (complete the
  mechanical edits). Add small unit tests per command group.
- Add `pipeline` abstraction and `bind_pipeline_command` (Phase 2).
- Add reflection/descriptor abstractions and `command_encoder` for Vulkan/Metal.

If you want I can implement the Phase 1 POC now: create the `command_executor`
files and update `functions.hpp`, build and run the smoke checks. Reply
"Yes — implement Phase 1 POC" to proceed.

---

Revision history
- 2025-08-31: Initial plan and coding-agent prompt added.
