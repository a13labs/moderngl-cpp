#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_core/debug.hpp"

namespace mgl::graphics
{
  template <typename T>
  class manager
  {
    struct database
    {
      mgl::unordered_map<std::string, T> items_by_name;
      mgl::unordered_map<size_t, T> items_by_idx;
      mgl::unordered_map<std::string, size_t> item_idx_by_name;
      mgl::unordered_map<size_t, std::string> item_name_by_idx;
      size_t item_idx = 1;

      size_t add_item(const std::string& name, const T& item)
      {
        if(items_by_name.find(name) != items_by_name.end())
          return 0;

        items_by_name[name] = item;
        items_by_idx[item_idx] = item;
        item_idx_by_name[name] = item_idx;
        item_name_by_idx[item_idx] = name;
        return item_idx++;
      }

      void remove_item(const std::string& name)
      {
        if(items_by_name.find(name) == items_by_name.end())
          return;

        items_by_name.erase(name);
        items_by_idx.erase(item_idx_by_name[name]);
        item_name_by_idx.erase(item_idx_by_name[name]);
        item_idx_by_name.erase(name);
      }

      void remove_item(size_t idx)
      {
        if(items_by_idx.find(idx) == items_by_idx.end())
          return;

        items_by_idx.erase(idx);
        items_by_name.erase(item_name_by_idx[idx]);
        item_idx_by_name.erase(item_name_by_idx[idx]);
        item_name_by_idx.erase(idx);
      }

      T& get_item(const std::string& name)
      {
        MGL_CORE_ASSERT(items_by_name.find(name) != items_by_name.end(), "Item does not exist");
        return items_by_name[name];
      }

      T& get_item(size_t idx)
      {
        MGL_CORE_ASSERT(items_by_idx.find(idx) != items_by_idx.end(), "Item does not exist");
        return items_by_idx[idx];
      }

      bool has_item(const std::string& name)
      {
        return items_by_name.find(name) != items_by_name.end();
      }

      void clear()
      {
        items_by_name.clear();
        items_by_idx.clear();
        item_idx_by_name.clear();
        item_name_by_idx.clear();
        item_idx = 0;
      }
    };

public:
    size_t add_item(const std::string& name, const T& item);
    void remove_item(const std::string& name);
    void remove_item(size_t idx);
    T& get_item(const std::string& name);
    T& get_item(size_t idx);
    bool has_item(const std::string& name);
    void clear();

protected:
    virtual void on_add(const T& item, const std::string& name) = 0;
    virtual void on_remove(const T& item, const std::string& name) = 0;

protected:
    database m_database;
  };

  template <typename T>
  size_t manager<T>::add_item(const std::string& name, const T& item)
  {
    size_t idx = m_database.add_item(name, item);
    if(idx == 0)
      return 0;
    on_add(item, name);
    return idx;
  }

  template <typename T>
  void manager<T>::remove_item(const std::string& name)
  {
    auto item = m_database.get_item(name);
    if(item == nullptr)
      return;

    on_remove(item, name);
    m_database.remove_item(name);
  }

  template <typename T>
  void manager<T>::remove_item(size_t idx)
  {
    auto item = m_database.get_item(idx);
    if(item == nullptr)
      return;

    on_remove(item);
    m_database.remove_item(idx);
  }

  template <typename T>
  T& manager<T>::get_item(const std::string& name)
  {
    return m_database.get_item(name);
  }

  template <typename T>
  T& manager<T>::get_item(size_t idx)
  {
    return m_database.get_item(idx);
  }

  template <typename T>
  void manager<T>::clear()
  {
    for(auto& item : m_database.items_by_name)
    {
      on_remove(item.second, item.first);
    }
    m_database.clear();
  }

  template <typename T>
  bool manager<T>::has_item(const std::string& name)
  {
    return m_database.has_item(name);
  }

} // namespace mgl::graphics