//  (C) Copyright Joel de Guzman 2003.
//  (C) Copyright Hartmut Kaiser 2006.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef LIST_INDEXING_SUITE_HK20061_HPP
# define LIST_INDEXING_SUITE_HK20061_HPP

# include <boost/python/suite/indexing/indexing_suite.hpp>
# include <boost/python/suite/indexing/container_utils.hpp>
# include <boost/python/iterator.hpp>

namespace boost { namespace python {
            
    // Forward declaration
    template <typename Container, bool NoProxy, typename DerivedPolicies>
    class list_indexing_suite;
    
    namespace detail
    {
        template <typename Container, bool NoProxy>
        struct final_list_derived_policies 
        :   public list_indexing_suite<
                Container, 
                NoProxy, 
                final_list_derived_policies<Container, NoProxy> 
            > 
        {};
    }

    // The list_indexing_suite class is a predefined indexing_suite derived 
    // class for wrapping std::list (and std::list like) classes. It provides
    // all the policies required by the indexing_suite (see indexing_suite).
    // Example usage:
    //
    //  class X {...};
    //
    //  ...
    //
    //      class_<std::list<X> >("XList")
    //          .def(list_indexing_suite<std::list<X> >())
    //      ;
    //
    // By default indexed elements are returned by proxy. This can be
    // disabled by supplying *true* in the NoProxy template parameter.
    //
    // Note, that the indexing operations required by Python are implemented 
    // with O(N) complexity.
    template <
        typename Container, 
        bool NoProxy = false,
        typename DerivedPolicies 
            = detail::final_list_derived_policies<Container, NoProxy> >
    class list_indexing_suite 
    :   public indexing_suite<Container, DerivedPolicies, NoProxy>
    {
    public:
    
        typedef typename Container::value_type data_type;
        typedef typename Container::value_type key_type;
        typedef typename Container::size_type index_type;
        typedef typename Container::size_type size_type;
        typedef typename Container::difference_type difference_type;
        
        typedef typename Container::iterator iterator_type;
        
        template <typename Class>
        static void 
        extension_def(Class& cl)
        {
            cl
                .def("append", &base_append)
                .def("extend", &base_extend)
            ;
        }
        
        static 
        typename mpl::if_<is_class<data_type>, data_type&, data_type>::type
        get_item(Container& container, index_type i)
        { 
            iterator_type it = container.begin();
            std::advance(it, i);
            return *it;
        }

        static object 
        get_slice(Container& container, index_type from, index_type to)
        { 
            if (from > to)
                return object(Container());

            iterator_type from_it = container.begin();
            iterator_type to_it = container.begin();

            std::advance(from_it, from);
            std::advance(to_it, to);
            return object(Container(from_it, to_it));
        }

        static void 
        set_item(Container& container, index_type i, data_type const& v)
        { 
            iterator_type it = container.begin();
            std::advance(it, i);
            *it = v;
        }

        static void 
        set_slice(Container& container, index_type from, 
            index_type to, data_type const& v)
        { 
            if (from > to) {
                return;
            }
            else {
                iterator_type from_it = container.begin();
                iterator_type to_it = container.begin();

                std::advance(from_it, from);
                std::advance(to_it, to);
                container.erase(from_it, to_it);
                
                from_it = container.begin();
                std::advance(from_it, from);
                container.insert(from_it, v);
            }
        }

        template <class Iter>
        static void 
        set_slice(Container& container, index_type from, 
            index_type to, Iter first, Iter last)
        { 
            iterator_type from_it = container.begin();

            std::advance(from_it, from);
            if (from > to) {
                container.insert(from_it, first, last);
            }
            else {
                iterator_type to_it = container.begin();

                std::advance(to_it, to);
                container.erase(from_it, to_it);

                from_it = container.begin();
                std::advance(from_it, from);
                container.insert(from_it, first, last);
            }
        }

        static void 
        delete_item(Container& container, index_type i)
        { 
            iterator_type it = container.begin();
            std::advance(it, i);
            container.erase(it);
        }
        
        static void 
        delete_slice(Container& container, index_type from, index_type to)
        { 
            if (from > to) {
                // A null-op.
                return;
            }

            iterator_type from_it = container.begin();
            iterator_type to_it = container.begin();

            std::advance(from_it, from);
            std::advance(to_it, to);
            container.erase(from_it, to_it);
        }
        
        static size_t
        size(Container& container)
        {
            return container.size();
        }
        
        static bool
        contains(Container& container, key_type const& key)
        {
            return std::find(container.begin(), container.end(), key)
                != container.end();
        }
        
        static index_type
        get_min_index(Container& container)
        { 
            return 0;
        }

        static index_type
        get_max_index(Container& container)
        { 
            return container.size();
        }
      
        static bool 
        compare_index(Container& container, index_type a, index_type b)
        {
            return a < b;
        }
        
        static index_type
        convert_index(Container& container, PyObject* i_)
        { 
            detail::do_python python_hold;
            extract<long> i(i_);
            if (i.check())
            {
                long index = i();
                if (index < 0)
                    index += DerivedPolicies::size(container);
                if (index >= long(container.size()) || index < 0)
                {
                    PyErr_SetString(PyExc_IndexError, "Index out of range");
                    throw_error_already_set();
                }
                return index;
            }
            
            PyErr_SetString(PyExc_TypeError, "Invalid index type");
            throw_error_already_set();
            return index_type();
        }
      
        static void 
        append(Container& container, data_type const& v)
        { 
            container.push_back(v);
        }
        
        template <class Iter>
        static void 
        extend(Container& container, Iter first, Iter last)
        { 
            container.insert(container.end(), first, last);
        }
        
    private:
    
        static void
        base_append(Container& container, object v)
        {
            extract<data_type&> elem(v);
            // try if elem is an exact Data
            if (elem.check())
            {
                DerivedPolicies::append(container, elem());
            }
            else
            {
                //  try to convert elem to data_type
                extract<data_type> elem(v);
                if (elem.check())
                {
                    DerivedPolicies::append(container, elem());
                }
                else
                {
                    PyErr_SetString(PyExc_TypeError, 
                        "Attempting to append an invalid type");
                    throw_error_already_set();
                }
            }
        }
        
        static void
        base_extend(Container& container, object v)
        {
            std::list<data_type> temp;
            container_utils::extend_container(temp, v);
            DerivedPolicies::extend(container, temp.begin(), temp.end());
        }
    };
       
}} // namespace boost::python 

#endif // LIST_INDEXING_SUITE_HK20061_HPP
