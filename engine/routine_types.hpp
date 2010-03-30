//  Copyright (c) 2005-2006 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(PYTHON_ROUTINE_TYPES_HPP)
#define PYTHON_ROUTINE_TYPES_HPP

#include <boost/python.hpp>
#include <saga/impl/exception.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace saga { namespace python 
{
    // possible routine types for the saga API functions
    enum routine_type
    {
        Sync = 1,
        Async = 2,
        Task = 3
    };

    ///////////////////////////////////////////////////////////////////////////
    struct python_callback
    {
        python_callback(boost::python::object f) 
        :   f_(f) 
        {}
        
        bool operator()(saga::object o, saga::metric m, saga::context c) const
        { 
            // make sure, this thread is known to the Python interpreter
            boost::python::detail::do_python wrap;
            return boost::python::call<bool>(f_.ptr(), o, m, c); 
        }
        
        boost::python::object f_;
    };

    struct python_callback_obj 
    {
        python_callback_obj (boost::python::object s, boost::python::object f) 
        :   s_(s), f_(f) 
        {}
        
        bool operator()(saga::object o, saga::metric m, saga::context c) const
        { 
            // make sure, this thread is known to the Python interpreter
            boost::python::detail::do_python wrap;
            return boost::python::call<bool>(f_.ptr(), s_, o, m, c); 
        }
        
        boost::python::object s_;
        boost::python::object f_;
    };
    
    ///////////////////////////////////////////////////////////////////////////
    //  convert a Python list into a C++ container
    template <typename Container>
    void from_list(boost::python::list l, Container& result)
    {
        typedef typename Container::value_type value_type;
        int n = boost::python::extract<int>(l.attr("__len__")());
        for (int i = 0; i < n; ++i) 
            result.push_back(boost::python::extract<value_type>(l[i]));
    }
    
///////////////////////////////////////////////////////////////////////////////
}}   // namespace saga::python

#if PY_VERSION_HEX < 0x02040000
// do not define the async API in Python Versions earlier than 2.4

///////////////////////////////////////////////////////////////////////////////
//  no parameters
#define DEFINE_PYTHON_WRAPPERS_0_EX(rt, target, func, sfunc)                  \
    static rt py_ ## func ## _0(target o)                                     \
    {                                                                         \
        return o.sfunc();                                                     \
    }                                                                         \
    static saga::task py_ ## func ## _task_0(target o,                        \
        saga::python::routine_type type)                                      \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync: return o.sfunc<saga::task_base::Sync>();     \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_0(rt, target, func)                            \
    DEFINE_PYTHON_WRAPPERS_0_EX(rt, target, func, func)                       \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_0_EX(func, sfunc, doc)                          \
    .def(#sfunc, py_ ## func ## _0, doc " (plain)")                           \
    .def(#sfunc, py_ ## func ## _task_0, doc " (task based)")                 \
    /**/
#define INSERT_PYTHON_WRAPPER_0(func, doc)                                    \
    INSERT_PYTHON_WRAPPER_0_EX(func, func, doc)                               \
    /**/

///////////////////////////////////////////////////////////////////////////////
//  1 parameter
#define DEFINE_PYTHON_WRAPPERS_1_EX(rt, target, func, sfunc, p1)              \
    static rt py_ ## func ## _1(target o, p1 par1)                            \
    {                                                                         \
        return o.sfunc(par1);                                                 \
    }                                                                         \
    static saga::task py_ ## func ## _task_1(target o,                        \
        saga::python::routine_type type, p1 par1)                             \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync: return o.sfunc<saga::task_base::Sync>(par1); \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_1(rt, target, func, p1)                        \
    DEFINE_PYTHON_WRAPPERS_1_EX(rt, target, func, func, p1)                   \
    /**/

#define DEFINE_PYTHON_WRAPPERS_1_DEF_1_EX(rt, target, func, sfunc, p1, v1)    \
    static rt py_ ## func ## _1(target o, p1 par1 = v1)                       \
    {                                                                         \
        return o.sfunc(par1);                                                 \
    }                                                                         \
    static saga::task py_ ## func ## _task_1(target o,                        \
        saga::python::routine_type type, p1 par1 = v1)                        \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync: return o.sfunc<saga::task_base::Sync>(par1); \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _1_ov,                     \
        py_ ## func ## _1, 1, 2)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_1_ov,                \
        py_ ## func ## _task_1, 2, 3)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_1_DEF_1(rt, target, func, p1, v1)              \
    DEFINE_PYTHON_WRAPPERS_1_DEF_1_EX(rt, target, func, func, p1, v1)         \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_1_EX(func, sfunc, doc)                          \
    .def(#sfunc, py_ ## func ## _1, doc " (plain)")                           \
    .def(#sfunc, py_ ## func ## _task_1, doc " (task based)")                 \
    /**/
#define INSERT_PYTHON_WRAPPER_1(func, doc)                                    \
    INSERT_PYTHON_WRAPPER_1_EX(func, func, doc)                               \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_1_EX_OVERLOADS(func, sfunc, doc)                \
    .def(#sfunc, py_ ## func ## _1, py_ ## func ## _1_ov(doc " (plain)"))     \
    .def(#sfunc, py_ ## func ## _task_1, py_ ## func ## _task_1_ov(           \
        doc " (task based)"))                                                 \
    /**/
#define INSERT_PYTHON_WRAPPER_1_OVERLOADS(func, doc)                          \
    INSERT_PYTHON_WRAPPER_1_EX_OVERLOADS(func, func, doc)                     \
    /**/

///////////////////////////////////////////////////////////////////////////////
//  2 parameters
#define DEFINE_PYTHON_WRAPPERS_2_EX(rt, target, func, sfunc, p1, p2)          \
    static rt py_ ## func ## _2(target o, p1 par1, p2 par2)                   \
    {                                                                         \
        return o.sfunc(par1, par2);                                           \
    }                                                                         \
    static saga::task py_ ## func ## _task_2(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2)                    \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2);                \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_2(rt, target, func, p1, p2)                    \
    DEFINE_PYTHON_WRAPPERS_2_EX(rt, target, func, func, p1, p2)               \
    /**/

#define DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(rt, target, func, sfunc, p1, p2, v2)\
    static rt py_ ## func ## _2(target o, p1 par1, p2 par2 = v2)              \
    {                                                                         \
        return o.sfunc(par1, par2);                                           \
    }                                                                         \
    static saga::task py_ ## func ## _task_2(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2 = v2)               \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2);                \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _2_ov,                     \
        py_ ## func ## _2, 2, 3)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_2_ov,                \
        py_ ## func ## _task_2, 3, 4)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_2_DEF_1(rt, target, func, p1, p2, v2)          \
    DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(rt, target, func, func, p1, p2, v2)     \
    /**/

#define DEFINE_PYTHON_WRAPPERS_2_DEF_2_EX(rt, target, func, sfunc, p1, v1,    \
        p2, v2)                                                               \
    static rt py_ ## func ## _2(target o, p1 par1 = v1, p2 par2 = v2)         \
    {                                                                         \
        return o.sfunc(par1, par2);                                           \
    }                                                                         \
    static saga::task py_ ## func ## _task_2(target o,                        \
        saga::python::routine_type type, p1 par1 = v1, p2 par2 = v2)          \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2);                \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _2_ov,                     \
        py_ ## func ## _2, 1, 3)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_2_ov,                \
        py_ ## func ## _task_2, 2, 4)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_2_DEF_2(rt, target, func, p1, v1, p2, v2)      \
    DEFINE_PYTHON_WRAPPERS_2_DEF_2_EX(rt, target, func, func, p1, v1, p2, v2) \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_2_EX(func, sfunc, doc)                          \
    .def(#sfunc, py_ ## func ## _2, doc " (plain)")                           \
    .def(#sfunc, py_ ## func ## _task_2, doc " (task based)")                 \
    /**/
#define INSERT_PYTHON_WRAPPER_2(func, doc)                                    \
    INSERT_PYTHON_WRAPPER_2_EX(func, func, doc)                               \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(func, sfunc, doc)                \
    .def(#sfunc, py_ ## func ## _2, py_ ## func ## _2_ov(doc " (plain)"))     \
    .def(#sfunc, py_ ## func ## _task_2, py_ ## func ## _task_2_ov(           \
        doc " (task based)"))                                                 \
    /**/
#define INSERT_PYTHON_WRAPPER_2_OVERLOADS(func, doc)                          \
    INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(func, func, doc)                     \
    /**/

///////////////////////////////////////////////////////////////////////////////
//  3 parameters
#define DEFINE_PYTHON_WRAPPERS_3_EX(rt, target, func, sfunc, p1, p2, p3)      \
    static rt py_ ## func ## _3(target o, p1 par1, p2 par2, p3 par3)          \
    {                                                                         \
        return o.sfunc(par1, par2, par3);                                     \
    }                                                                         \
    static saga::task py_ ## func ## _task_3(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3)           \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3);          \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_3(rt, target, func, p1, p2, p3)                \
    DEFINE_PYTHON_WRAPPERS_3_EX(rt, target, func, func, p1, p2, p3)           \
    /**/

#define DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(rt, target, func, sfunc, p1, p2,    \
        p3, v3)                                                               \
    static rt py_ ## func ## _3(target o, p1 par1, p2 par2, p3 par3 = v3)     \
    {                                                                         \
        return o.sfunc(par1, par2, par3);                                     \
    }                                                                         \
    static saga::task py_ ## func ## _task_3(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3 = v3)      \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3);          \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _3_ov,                     \
        py_ ## func ## _3, 3, 4)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_3_ov,                \
        py_ ## func ## _task_3, 4, 5)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_3_DEF_1(rt, target, func, p1, p2, p3, v3)      \
    DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(rt, target, func, func, p1, p2, p3, v3) \
    /**/

#define DEFINE_PYTHON_WRAPPERS_3_DEF_2_EX(rt, target, func, sfunc, p1, p2, v2,\
      p3, v3)                                                                 \
    static rt py_ ## func ## _3(target o, p1 par1, p2 par2 = v2, p3 par3 = v3)\
    {                                                                         \
        return o.sfunc(par1, par2, par3);                                     \
    }                                                                         \
    static saga::task py_ ## func ## _task_3(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2 = v2, p3 par3 = v3) \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3);          \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _3_ov,                     \
        py_ ## func ## _3, 2, 4)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_3_ov,                \
        py_ ## func ## _task_3, 3, 5)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_3_DEF_2(rt, target, func, p1, p2, v2, p3, v3)  \
    DEFINE_PYTHON_WRAPPERS_3_DEF_2_EX(rt, target, func, func, p1, p2, v2,     \
        p3, v3)                                                               \
    /**/

#define DEFINE_PYTHON_WRAPPERS_3_DEF_3_EX(rt, target, func, sfunc, p1, v1,    \
        p2, v2, p3, v3)                                                       \
    static rt py_ ## func ## _3(target o, p1 par1 = v1, p2 par2 = v2, p3 par3 = v3)\
    {                                                                         \
        return o.sfunc(par1, par2, par3);                                     \
    }                                                                         \
    static saga::task py_ ## func ## _task_3(target o,                        \
        saga::python::routine_type type, p1 par1 = v1, p2 par2 = v2,          \
        p3 par3 = v3)                                                         \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3);          \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _3_ov,                     \
        py_ ## func ## _3, 1, 4)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_3_ov,                \
        py_ ## func ## _task_3, 2, 5)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_3_DEF_3(rt, target, func, p1, v1, p2, v2,      \
        p3, v3)                                                               \
    DEFINE_PYTHON_WRAPPERS_3_DEF_2_EX(rt, target, func, func, p1, v1, p2, v2, \
        p3, v3)                                                               \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_3_EX(func, sfunc, doc)                          \
    .def(#sfunc, py_ ## func ## _3, doc " (plain)")                           \
    .def(#sfunc, py_ ## func ## _task_3, doc " (task based)")                 \
    /**/    
#define INSERT_PYTHON_WRAPPER_3(func, doc)                                    \
    INSERT_PYTHON_WRAPPER_3_EX(func, func, doc)                               \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(func, sfunc, doc)                \
    .def(#sfunc, py_ ## func ## _3, py_ ## func ## _3_ov(doc " (plain)"))     \
    .def(#sfunc, py_ ## func ## _task_3, py_ ## func ## _task_3_ov(           \
        doc " (task based)"))                                                 \
    /**/
#define INSERT_PYTHON_WRAPPER_3_OVERLOADS(func, doc)                          \
    INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(func, func, doc)                     \
    /**/

///////////////////////////////////////////////////////////////////////////////
//  4 parameters
#define DEFINE_PYTHON_WRAPPERS_4_EX(rt, target, func, sfunc, p1, p2, p3, p4)  \
    static rt py_ ## func ## _4(target o, p1 par1, p2 par2, p3 par3, p4 par4) \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4);                               \
    }                                                                         \
    static saga::task py_ ## func ## _task_4(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3, p4 par4)  \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4);    \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_4(rt, target, func, p1, p2, p3, p4)            \
    DEFINE_PYTHON_WRAPPERS_4_EX(rt, target, func, func, p1, p2, p3, p4)       \
    /**/

#define DEFINE_PYTHON_WRAPPERS_4_DEF_1_EX(rt, target, func, sfunc, p1, p2, p3,\
        p4, v4)                                                               \
    static rt py_ ## func ## _4(target o, p1 par1, p2 par2, p3 par3,          \
        p4 par4 = v4)                                                         \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4);                               \
    }                                                                         \
    static saga::task py_ ## func ## _task_4(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3,           \
        p4 par4 = v4)                                                         \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4);    \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _4_ov,                     \
        py_ ## func ## _4, 4, 5)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_4_ov,                \
        py_ ## func ## _task_4, 5, 6)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_4_DEF_1(rt, target, func, p1, p2, p3, p4, v4)  \
    DEFINE_PYTHON_WRAPPERS_4_DEF_1_EX(rt, target, func, func, p1, p2, p3,     \
        p4, v4)                                                               \
    /**/

#define DEFINE_PYTHON_WRAPPERS_4_DEF_2_EX(rt, target, func, sfunc, p1, p2,    \
        p3, v3, p4, v4)                                                       \
    static rt py_ ## func ## _4(target o, p1 par1, p2 par2, p3 par3 = v3,     \
        p4 par4 = v4)                                                         \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4);                               \
    }                                                                         \
    static saga::task py_ ## func ## _task_4(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3 = v3,      \
        p4 par4 = v4)                                                         \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4);    \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _4_ov,                     \
        py_ ## func ## _4, 3, 5)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_4_ov,                \
        py_ ## func ## _task_4, 4, 6)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_4_DEF_2(rt, target, func, p1, p2, p3, v3,      \
        p4, v4)                                                               \
    DEFINE_PYTHON_WRAPPERS_4_DEF_2_EX(rt, target, func, func, p1, p2, p3, v3, \
        p4, v4)                                                               \
    /**/

#define DEFINE_PYTHON_WRAPPERS_4_DEF_3_EX(rt, target, func, sfunc, p1, p2, v2,\
        p3, v3, p4, v4)                                                       \
    static rt py_ ## func ## _4(target o, p1 par1, p2 par2 = v2, p3 par3 = v3,\
        p4 par4 = v4)                                                         \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4);                               \
    }                                                                         \
    static saga::task py_ ## func ## _task_4(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2 = v2, p3 par3 = v3, \
        p4 par4 = v4)                                                         \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4);    \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _4_ov,                     \
        py_ ## func ## _4, 2, 5)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_4_ov,                \
        py_ ## func ## _task_4, 3, 6)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_4_DEF_3(rt, target, func, p1, p2, v2, p3, v3,  \
        p4, v4)                                                               \
    DEFINE_PYTHON_WRAPPERS_4_DEF_3_EX(rt, target, func, func, p1, p2, v2,     \
        p3, v3, p4, v4)                                                       \
    /**/

#define DEFINE_PYTHON_WRAPPERS_4_DEF_4_EX(rt, target, func, sfunc, p1, v1,    \
        p2, v2, p3, v3, p4, v4)                                               \
    static rt py_ ## func ## _4(target o, p1 par1 = v1, p2 par2 = v2,         \
        p3 par3 = v3, p4 par4 = v4)                                           \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4);                               \
    }                                                                         \
    static saga::task py_ ## func ## _task_4(target o,                        \
        saga::python::routine_type type, p1 par1 = v1, p2 par2 = v2,          \
        p3 par3 = v3, p4 par4 = v4)                                           \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4);    \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _4_ov,                     \
        py_ ## func ## _4, 1, 5)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_4_ov,                \
        py_ ## func ## _task_4, 2, 6)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_4_DEF_4(rt, target, func, p1, v1, p2, v2,      \
        p3, v3, p4, v4)                                                       \
    DEFINE_PYTHON_WRAPPERS_4_DEF_4_EX(rt, target, func, func, p1, v1, p2, v2, \
        p3, v3, p4, v4)                                                       \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_4_EX(func, sfunc, doc)                          \
    .def(#sfunc, py_ ## func ## _4, doc " (plain)")                           \
    .def(#sfunc, py_ ## func ## _task_4, doc " (task based)")                 \
    /**/    
#define INSERT_PYTHON_WRAPPER_4(func, doc)                                    \
    INSERT_PYTHON_WRAPPER_4_EX(func, func, doc)                               \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_4_EX_OVERLOADS(func, sfunc, doc)                \
    .def(#sfunc, py_ ## func ## _4, py_ ## func ## _4_ov(doc " (plain)"))     \
    .def(#sfunc, py_ ## func ## _task_4, py_ ## func ## _task_4_ov(           \
        doc " (task based)"))                                                 \
    /**/
#define INSERT_PYTHON_WRAPPER_4_OVERLOADS(func, doc)                          \
    INSERT_PYTHON_WRAPPER_4_EX_OVERLOADS(func, func, doc)                     \
    /**/

///////////////////////////////////////////////////////////////////////////////
//  5 parameters
#define DEFINE_PYTHON_WRAPPERS_5_EX(rt, target, func, sfunc, p1, p2, p3, p4, p5)\
    static rt py_ ## func ## _5(target o, p1 par1, p2 par2, p3 par3, p4 par4, \
        p5 par5)                                                              \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4, par5);                         \
    }                                                                         \
    static saga::task py_ ## func ## _task_5(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3, p4 par4,  \
        p5 par5)                                                              \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4, par5); \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_5(rt, target, func, p1, p2, p3, p4, p5)        \
    DEFINE_PYTHON_WRAPPERS_5_EX(rt, target, func, func, p1, p2, p3, p4, p5)   \
    /**/

#define DEFINE_PYTHON_WRAPPERS_5_DEF_1_EX(rt, target, func, sfunc, p1, p2,    \
        p3, p4, p5, v5)                                                       \
    static rt py_ ## func ## _5(target o, p1 par1, p2 par2, p3 par3,          \
        p4 par4, p5 par5 = v5)                                                \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4, par5);                         \
    }                                                                         \
    static saga::task py_ ## func ## _task_5(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3, p4 par4,  \
        p5 par5 = v5)                                                         \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4, par5); \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _5_ov,                     \
        py_ ## func ## _5, 5, 6)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_5_ov,                \
        py_ ## func ## _task_5, 6, 7)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_5_DEF_1(rt, target, func, p1, p2, p3, p4, p5, v5)\
    DEFINE_PYTHON_WRAPPERS_5_DEF_1_EX(rt, target, func, func, p1, p2, p3,     \
        p4, p5, v5)                                                           \
    /**/
    
#define DEFINE_PYTHON_WRAPPERS_5_DEF_2_EX(rt, target, func, sfunc, p1, p2, p3,\
        p4, v4, p5, v5)                                                       \
    static rt py_ ## func ## _5(target o, p1 par1, p2 par2, p3 par3,          \
        p4 par4 = v4, p5 par5 = v5)                                           \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4, par5);                         \
    }                                                                         \
    static saga::task py_ ## func ## _task_5(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3,           \
        p4 par4 = v4, p5 par5 = v5)                                           \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4, par5); \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, AsAsy or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _5_ov,                     \
        py_ ## func ## _5, 4, 6)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_5_ov,                \
        py_ ## func ## _task_5, 5, 7)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_5_DEF_2(rt, target, func, p1, p2, p3, p4, v4,  \
        p5, v5)                                                               \
    DEFINE_PYTHON_WRAPPERS_5_DEF_2_EX(rt, target, func, func, p1, p2, p3,     \
        p4, v4, p5, v5)                                                       \
    /**/

#define DEFINE_PYTHON_WRAPPERS_5_DEF_3_EX(rt, target, func, sfunc, p1, p2,    \
        p3, v3, p4, v4, p5, v5)                                               \
    static rt py_ ## func ## _5(target o, p1 par1, p2 par2, p3 par3 = v3,     \
        p4 par4 = v4, p5 par5 = v5)                                           \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4, par5);                         \
    }                                                                         \
    static saga::task py_ ## func ## _task_5(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3 = v3,      \
        p4 par4 = v4, p5 par5 = v5)                                           \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4, par5);\
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _5_ov,                     \
        py_ ## func ## _5, 3, 6)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_5_ov,                \
        py_ ## func ## _task_5, 4, 7)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_5_DEF_3(rt, target, func, p1, p2, p3, v3,      \
        p4, v4, p5, v5)                                                       \
    DEFINE_PYTHON_WRAPPERS_5_DEF_3_EX(rt, target, func, func, p1, p2, p3, v3, \
        p4, v4, p5, v5)                                                       \
    /**/

#define DEFINE_PYTHON_WRAPPERS_5_DEF_4_EX(rt, target, func, sfunc, p1, p2, v2,\
        p3, v3, p4, v4, p5, v5)                                               \
    static rt py_ ## func ## _5(target o, p1 par1, p2 par2 = v2,              \
        p3 par3 = v3, p4 par4 = v4, p5 par5 = v5)                             \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4, par5);                         \
    }                                                                         \
    static saga::task py_ ## func ## _task_5(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2 = v2, p3 par3 = v3, \
        p4 par4 = v4, p5 par5 = v5)                                           \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4, par5);\
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _5_ov,                     \
        py_ ## func ## _5, 2, 6)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_5_ov,                \
        py_ ## func ## _task_5, 3, 7)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_5_DEF_4(rt, target, func, p1, p2, v2, p3, v3,  \
        p4, v4, p5, v5)                                                       \
    DEFINE_PYTHON_WRAPPERS_5_DEF_4_EX(rt, target, func, func, p1, p2, v2,     \
        p3, v3, p4, v4, p5, v5)                                               \
    /**/

#define DEFINE_PYTHON_WRAPPERS_5_DEF_5_EX(rt, target, func, sfunc, p1, v1,    \
        p2, v2, p3, v3, p4, v4, p5, v5)                                       \
    static rt py_ ## func ## _5(target o, p1 par1 = v1, p2 par2 = v2,         \
        p3 par3 = v3, p4 par4 = v4, p5 par5 = v5)                             \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4, par5);                         \
    }                                                                         \
    static saga::task py_ ## func ## _task_5(target o,                        \
        saga::python::routine_type type, p1 par1 = v1, p2 par2 = v2,          \
        p3 par3 = v3, p4 par4 = v4, p5 par5 = v5)                             \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4, par5); \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _5_ov,                     \
        py_ ## func ## _5, 1, 6)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_5_ov,                \
        py_ ## func ## _task_5, 2, 7)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_5_DEF_5(rt, target, func, p1, v1, p2, v2,      \
        p3, v3, p4, v4, p5, v5)                                               \
    DEFINE_PYTHON_WRAPPERS_5_DEF_4_EX(rt, target, func, func, p1, v1, p2, v2, \
        p3, v3, p4, v4, p5, v5)                                               \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_5_EX(func, sfunc, doc)                          \
    .def(#sfunc, py_ ## func ## _5, doc " (plain)")                           \
    .def(#sfunc, py_ ## func ## _task_5, doc " (task based)")                 \
    /**/    
#define INSERT_PYTHON_WRAPPER_5(func, doc)                                    \
    INSERT_PYTHON_WRAPPER_5_EX(func, func, doc)                               \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_5_EX_OVERLOADS(func, sfunc, doc)                \
    .def(#sfunc, py_ ## func ## _5, py_ ## func ## _5_ov(doc " (plain)"))     \
    .def(#sfunc, py_ ## func ## _task_5, py_ ## func ## _task_5_ov(           \
        doc " (task based)"))                                                 \
    /**/
#define INSERT_PYTHON_WRAPPER_5_OVERLOADS(func, doc)                          \
    INSERT_PYTHON_WRAPPER_5_EX_OVERLOADS(func, func, doc)                     \
    /**/

#else

///////////////////////////////////////////////////////////////////////////////
//  no parameters
#define DEFINE_PYTHON_WRAPPERS_0_EX(rt, target, func, sfunc)                  \
    static rt py_ ## func ## _0(target o)                                     \
    {                                                                         \
        return o.sfunc();                                                     \
    }                                                                         \
    static saga::task py_ ## func ## _task_0(target o,                        \
        saga::python::routine_type type)                                      \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync: return o.sfunc<saga::task_base::Sync>();     \
        case saga::python::Async: return o.sfunc<saga::task_base::Async>();   \
        case saga::python::Task: return o.sfunc<saga::task_base::Task>();     \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_0(rt, target, func)                            \
    DEFINE_PYTHON_WRAPPERS_0_EX(rt, target, func, func)                       \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_0_EX(func, sfunc, doc)                          \
    .def(#sfunc, py_ ## func ## _0, doc " (plain)")                           \
    .def(#sfunc, py_ ## func ## _task_0, doc " (task based)")                 \
    /**/
#define INSERT_PYTHON_WRAPPER_0(func, doc)                                    \
    INSERT_PYTHON_WRAPPER_0_EX(func, func, doc)                               \
    /**/
    
///////////////////////////////////////////////////////////////////////////////
//  1 parameter
#define DEFINE_PYTHON_WRAPPERS_1_EX(rt, target, func, sfunc, p1)              \
    static rt py_ ## func ## _1(target o, p1 par1)                            \
    {                                                                         \
        return o.sfunc(par1);                                                 \
    }                                                                         \
    static saga::task py_ ## func ## _task_1(target o,                        \
        saga::python::routine_type type, p1 par1)                             \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync: return o.sfunc<saga::task_base::Sync>(par1); \
        case saga::python::Async: return o.sfunc<saga::task_base::Async>(par1);\
        case saga::python::Task: return o.sfunc<saga::task_base::Task>(par1); \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_1(rt, target, func, p1)                        \
    DEFINE_PYTHON_WRAPPERS_1_EX(rt, target, func, func, p1)                   \
    /**/

#define DEFINE_PYTHON_WRAPPERS_1_DEF_1_EX(rt, target, func, sfunc, p1, v1)    \
    static rt py_ ## func ## _1(target o, p1 par1 = v1)                       \
    {                                                                         \
        return o.sfunc(par1);                                                 \
    }                                                                         \
    static saga::task py_ ## func ## _task_1(target o,                        \
        saga::python::routine_type type, p1 par1 = v1)                        \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync: return o.sfunc<saga::task_base::Sync>(par1); \
        case saga::python::Async: return o.sfunc<saga::task_base::Async>(par1);\
        case saga::python::Task: return o.sfunc<saga::task_base::Task>(par1); \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _1_ov,                     \
        py_ ## func ## _1, 1, 2)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_1_ov,                \
        py_ ## func ## _task_1, 2, 3)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_1_DEF_1(rt, target, func, p1, v1)              \
    DEFINE_PYTHON_WRAPPERS_1_DEF_1_EX(rt, target, func, func, p1, v1)         \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_1_EX(func, sfunc, doc)                          \
    .def(#sfunc, py_ ## func ## _1, doc " (plain)")                           \
    .def(#sfunc, py_ ## func ## _task_1, doc " (task based)")                 \
    /**/
#define INSERT_PYTHON_WRAPPER_1(func, doc)                                    \
    INSERT_PYTHON_WRAPPER_1_EX(func, func, doc)                               \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_1_EX_OVERLOADS(func, sfunc, doc)                \
    .def(#sfunc, py_ ## func ## _1, py_ ## func ## _1_ov(doc " (plain)"))     \
    .def(#sfunc, py_ ## func ## _task_1, py_ ## func ## _task_1_ov(           \
        doc " (task based)"))                                                 \
    /**/
#define INSERT_PYTHON_WRAPPER_1_OVERLOADS(func, doc)                          \
    INSERT_PYTHON_WRAPPER_1_EX_OVERLOADS(func, func, doc)                     \
    /**/

///////////////////////////////////////////////////////////////////////////////
//  2 parameters
#define DEFINE_PYTHON_WRAPPERS_2_EX(rt, target, func, sfunc, p1, p2)          \
    static rt py_ ## func ## _2(target o, p1 par1, p2 par2)                   \
    {                                                                         \
        return o.sfunc(par1, par2);                                           \
    }                                                                         \
    static saga::task py_ ## func ## _task_2(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2)                    \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2);                \
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2);               \
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2);                \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_2(rt, target, func, p1, p2)                    \
    DEFINE_PYTHON_WRAPPERS_2_EX(rt, target, func, func, p1, p2)               \
    /**/

#define DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(rt, target, func, sfunc, p1, p2, v2)\
    static rt py_ ## func ## _2(target o, p1 par1, p2 par2 = v2)              \
    {                                                                         \
        return o.sfunc(par1, par2);                                           \
    }                                                                         \
    static saga::task py_ ## func ## _task_2(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2 = v2)               \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2);                \
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2);               \
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2);                \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _2_ov,                     \
        py_ ## func ## _2, 2, 3)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_2_ov,                \
        py_ ## func ## _task_2, 3, 4)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_2_DEF_1(rt, target, func, p1, p2, v2)          \
    DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(rt, target, func, func, p1, p2, v2)     \
    /**/

#define DEFINE_PYTHON_WRAPPERS_2_DEF_2_EX(rt, target, func, sfunc, p1, v1,    \
        p2, v2)                                                               \
    static rt py_ ## func ## _2(target o, p1 par1 = v1, p2 par2 = v2)         \
    {                                                                         \
        return o.sfunc(par1, par2);                                           \
    }                                                                         \
    static saga::task py_ ## func ## _task_2(target o,                        \
        saga::python::routine_type type, p1 par1 = v1, p2 par2 = v2)          \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2);                \
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2);               \
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2);                \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _2_ov,                     \
        py_ ## func ## _2, 1, 3)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_2_ov,                \
        py_ ## func ## _task_2, 2, 4)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_2_DEF_2(rt, target, func, p1, v1, p2, v2)      \
    DEFINE_PYTHON_WRAPPERS_2_DEF_2_EX(rt, target, func, func, p1, v1, p2, v2) \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_2_EX(func, sfunc, doc)                          \
    .def(#sfunc, py_ ## func ## _2, doc " (plain)")                           \
    .def(#sfunc, py_ ## func ## _task_2, doc " (task based)")                 \
    /**/
#define INSERT_PYTHON_WRAPPER_2(func, doc)                                    \
    INSERT_PYTHON_WRAPPER_2_EX(func, func, doc)                               \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(func, sfunc, doc)                \
    .def(#sfunc, py_ ## func ## _2, py_ ## func ## _2_ov(doc " (plain)"))     \
    .def(#sfunc, py_ ## func ## _task_2, py_ ## func ## _task_2_ov(           \
        doc " (task based)"))                                                 \
    /**/
#define INSERT_PYTHON_WRAPPER_2_OVERLOADS(func, doc)                          \
    INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(func, func, doc)                     \
    /**/

///////////////////////////////////////////////////////////////////////////////
//  3 parameters
#define DEFINE_PYTHON_WRAPPERS_3_EX(rt, target, func, sfunc, p1, p2, p3)      \
    static rt py_ ## func ## _3(target o, p1 par1, p2 par2, p3 par3)          \
    {                                                                         \
        return o.sfunc(par1, par2, par3);                                     \
    }                                                                         \
    static saga::task py_ ## func ## _task_3(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3)           \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3);          \
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2, par3);         \
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2, par3);          \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_3(rt, target, func, p1, p2, p3)                \
    DEFINE_PYTHON_WRAPPERS_3_EX(rt, target, func, func, p1, p2, p3)           \
    /**/

#define DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(rt, target, func, sfunc, p1, p2,    \
        p3, v3)                                                               \
    static rt py_ ## func ## _3(target o, p1 par1, p2 par2, p3 par3 = v3)     \
    {                                                                         \
        return o.sfunc(par1, par2, par3);                                     \
    }                                                                         \
    static saga::task py_ ## func ## _task_3(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3 = v3)      \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3);          \
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2, par3);         \
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2, par3);          \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _3_ov,                     \
        py_ ## func ## _3, 3, 4)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_3_ov,                \
        py_ ## func ## _task_3, 4, 5)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_3_DEF_1(rt, target, func, p1, p2, p3, v3)      \
    DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(rt, target, func, func, p1, p2, p3, v3) \
    /**/

#define DEFINE_PYTHON_WRAPPERS_3_DEF_2_EX(rt, target, func, sfunc, p1, p2, v2,\
      p3, v3)                                                                 \
    static rt py_ ## func ## _3(target o, p1 par1, p2 par2 = v2, p3 par3 = v3)\
    {                                                                         \
        return o.sfunc(par1, par2, par3);                                     \
    }                                                                         \
    static saga::task py_ ## func ## _task_3(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2 = v2, p3 par3 = v3) \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3);          \
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2, par3);         \
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2, par3);          \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _3_ov,                     \
        py_ ## func ## _3, 2, 4)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_3_ov,                \
        py_ ## func ## _task_3, 3, 5)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_3_DEF_2(rt, target, func, p1, p2, v2, p3, v3)  \
    DEFINE_PYTHON_WRAPPERS_3_DEF_2_EX(rt, target, func, func, p1, p2, v2,     \
        p3, v3)                                                               \
    /**/

#define DEFINE_PYTHON_WRAPPERS_3_DEF_3_EX(rt, target, func, sfunc, p1, v1,    \
        p2, v2, p3, v3)                                                       \
    static rt py_ ## func ## _3(target o, p1 par1 = v1, p2 par2 = v2, p3 par3 = v3)\
    {                                                                         \
        return o.sfunc(par1, par2, par3);                                     \
    }                                                                         \
    static saga::task py_ ## func ## _task_3(target o,                        \
        saga::python::routine_type type, p1 par1 = v1, p2 par2 = v2,          \
        p3 par3 = v3)                                                         \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3);          \
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2, par3);         \
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2, par3);          \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _3_ov,                     \
        py_ ## func ## _3, 1, 4)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_3_ov,                \
        py_ ## func ## _task_3, 2, 5)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_3_DEF_3(rt, target, func, p1, v1, p2, v2,      \
        p3, v3)                                                               \
    DEFINE_PYTHON_WRAPPERS_3_DEF_2_EX(rt, target, func, func, p1, v1, p2, v2, \
        p3, v3)                                                               \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_3_EX(func, sfunc, doc)                          \
    .def(#sfunc, py_ ## func ## _3, doc " (plain)")                           \
    .def(#sfunc, py_ ## func ## _task_3, doc " (task based)")                 \
    /**/    
#define INSERT_PYTHON_WRAPPER_3(func, doc)                                    \
    INSERT_PYTHON_WRAPPER_3_EX(func, func, doc)                               \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(func, sfunc, doc)                \
    .def(#sfunc, py_ ## func ## _3, py_ ## func ## _3_ov(doc " (plain)"))     \
    .def(#sfunc, py_ ## func ## _task_3, py_ ## func ## _task_3_ov(           \
        doc " (task based)"))                                                 \
    /**/
#define INSERT_PYTHON_WRAPPER_3_OVERLOADS(func, doc)                          \
    INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(func, func, doc)                     \
    /**/

///////////////////////////////////////////////////////////////////////////////
//  4 parameters
#define DEFINE_PYTHON_WRAPPERS_4_EX(rt, target, func, sfunc, p1, p2, p3, p4)  \
    static rt py_ ## func ## _4(target o, p1 par1, p2 par2, p3 par3, p4 par4) \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4);                               \
    }                                                                         \
    static saga::task py_ ## func ## _task_4(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3, p4 par4)  \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4);    \
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2, par3, par4);   \
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2, par3, par4);    \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_4(rt, target, func, p1, p2, p3, p4)            \
    DEFINE_PYTHON_WRAPPERS_4_EX(rt, target, func, func, p1, p2, p3, p4)       \
    /**/

#define DEFINE_PYTHON_WRAPPERS_4_DEF_1_EX(rt, target, func, sfunc, p1, p2, p3,\
        p4, v4)                                                               \
    static rt py_ ## func ## _4(target o, p1 par1, p2 par2, p3 par3,          \
        p4 par4 = v4)                                                         \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4);                               \
    }                                                                         \
    static saga::task py_ ## func ## _task_4(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3,           \
        p4 par4 = v4)                                                         \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4);    \
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2, par3, par4);   \
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2, par3, par4);    \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _4_ov,                     \
        py_ ## func ## _4, 4, 5)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_4_ov,                \
        py_ ## func ## _task_4, 5, 6)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_4_DEF_1(rt, target, func, p1, p2, p3, p4, v4)  \
    DEFINE_PYTHON_WRAPPERS_4_DEF_1_EX(rt, target, func, func, p1, p2, p3,     \
        p4, v4)                                                               \
    /**/

#define DEFINE_PYTHON_WRAPPERS_4_DEF_2_EX(rt, target, func, sfunc, p1, p2,    \
        p3, v3, p4, v4)                                                       \
    static rt py_ ## func ## _4(target o, p1 par1, p2 par2, p3 par3 = v3,     \
        p4 par4 = v4)                                                         \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4);                               \
    }                                                                         \
    static saga::task py_ ## func ## _task_4(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3 = v3,      \
        p4 par4 = v4)                                                         \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4);    \
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2, par3, par4);   \
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2, par3, par4);    \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _4_ov,                     \
        py_ ## func ## _4, 3, 5)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_4_ov,                \
        py_ ## func ## _task_4, 4, 6)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_4_DEF_2(rt, target, func, p1, p2, p3, v3,      \
        p4, v4)                                                               \
    DEFINE_PYTHON_WRAPPERS_4_DEF_2_EX(rt, target, func, func, p1, p2, p3, v3, \
        p4, v4)                                                               \
    /**/

#define DEFINE_PYTHON_WRAPPERS_4_DEF_3_EX(rt, target, func, sfunc, p1, p2, v2,\
        p3, v3, p4, v4)                                                       \
    static rt py_ ## func ## _4(target o, p1 par1, p2 par2 = v2, p3 par3 = v3,\
        p4 par4 = v4)                                                         \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4);                               \
    }                                                                         \
    static saga::task py_ ## func ## _task_4(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2 = v2, p3 par3 = v3, \
        p4 par4 = v4)                                                         \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4);    \
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2, par3, par4);   \
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2, par3, par4);    \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _4_ov,                     \
        py_ ## func ## _4, 2, 5)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_4_ov,                \
        py_ ## func ## _task_4, 3, 6)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_4_DEF_3(rt, target, func, p1, p2, v2, p3, v3,  \
        p4, v4)                                                               \
    DEFINE_PYTHON_WRAPPERS_4_DEF_3_EX(rt, target, func, func, p1, p2, v2,     \
        p3, v3, p4, v4)                                                       \
    /**/

#define DEFINE_PYTHON_WRAPPERS_4_DEF_4_EX(rt, target, func, sfunc, p1, v1,    \
        p2, v2, p3, v3, p4, v4)                                               \
    static rt py_ ## func ## _4(target o, p1 par1 = v1, p2 par2 = v2,         \
        p3 par3 = v3, p4 par4 = v4)                                           \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4);                               \
    }                                                                         \
    static saga::task py_ ## func ## _task_4(target o,                        \
        saga::python::routine_type type, p1 par1 = v1, p2 par2 = v2,          \
        p3 par3 = v3, p4 par4 = v4)                                           \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4);    \
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2, par3, par4);   \
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2, par3, par4);    \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _4_ov,                     \
        py_ ## func ## _4, 1, 5)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_4_ov,                \
        py_ ## func ## _task_4, 2, 6)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_4_DEF_4(rt, target, func, p1, v1, p2, v2,      \
        p3, v3, p4, v4)                                                       \
    DEFINE_PYTHON_WRAPPERS_4_DEF_4_EX(rt, target, func, func, p1, v1, p2, v2, \
        p3, v3, p4, v4)                                                       \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_4_EX(func, sfunc, doc)                          \
    .def(#sfunc, py_ ## func ## _4, doc " (plain)")                           \
    .def(#sfunc, py_ ## func ## _task_4, doc " (task based)")                 \
    /**/    
#define INSERT_PYTHON_WRAPPER_4(func, doc)                                    \
    INSERT_PYTHON_WRAPPER_4_EX(func, func, doc)                               \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_4_EX_OVERLOADS(func, sfunc, doc)                \
    .def(#sfunc, py_ ## func ## _4, py_ ## func ## _4_ov(doc " (plain)"))     \
    .def(#sfunc, py_ ## func ## _task_4, py_ ## func ## _task_4_ov(           \
        doc " (task based)"))                                                 \
    /**/
#define INSERT_PYTHON_WRAPPER_4_OVERLOADS(func, doc)                          \
    INSERT_PYTHON_WRAPPER_4_EX_OVERLOADS(func, func, doc)                     \
    /**/

///////////////////////////////////////////////////////////////////////////////
//  5 parameters
#define DEFINE_PYTHON_WRAPPERS_5_EX(rt, target, func, sfunc, p1, p2, p3, p4, p5)\
    static rt py_ ## func ## _5(target o, p1 par1, p2 par2, p3 par3, p4 par4, \
        p5 par5)                                                              \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4, par5);                         \
    }                                                                         \
    static saga::task py_ ## func ## _task_5(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3, p4 par4,  \
        p5 par5)                                                              \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4, par5); \
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2, par3, par4, par5);\
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2, par3, par4, par5); \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_5(rt, target, func, p1, p2, p3, p4, p5)        \
    DEFINE_PYTHON_WRAPPERS_5_EX(rt, target, func, func, p1, p2, p3, p4, p5)   \
    /**/

#define DEFINE_PYTHON_WRAPPERS_5_DEF_1_EX(rt, target, func, sfunc, p1, p2,    \
        p3, p4, p5, v5)                                                       \
    static rt py_ ## func ## _5(target o, p1 par1, p2 par2, p3 par3,          \
        p4 par4, p5 par5 = v5)                                                \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4, par5);                         \
    }                                                                         \
    static saga::task py_ ## func ## _task_5(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3, p4 par4,  \
        p5 par5 = v5)                                                         \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4, par5); \
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2, par3, par4, par5);\
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2, par3, par4, par5); \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _5_ov,                     \
        py_ ## func ## _5, 5, 6)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_5_ov,                \
        py_ ## func ## _task_5, 6, 7)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_5_DEF_1(rt, target, func, p1, p2, p3, p4, p5, v5)\
    DEFINE_PYTHON_WRAPPERS_5_DEF_1_EX(rt, target, func, func, p1, p2, p3,     \
        p4, p5, v5)                                                           \
    /**/
    
#define DEFINE_PYTHON_WRAPPERS_5_DEF_2_EX(rt, target, func, sfunc, p1, p2, p3,\
        p4, v4, p5, v5)                                                       \
    static rt py_ ## func ## _5(target o, p1 par1, p2 par2, p3 par3,          \
        p4 par4 = v4, p5 par5 = v5)                                           \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4, par5);                         \
    }                                                                         \
    static saga::task py_ ## func ## _task_5(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3,           \
        p4 par4 = v4, p5 par5 = v5)                                           \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4, par5); \
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2, par3, par4, par5);\
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2, par3, par4, par5); \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _5_ov,                     \
        py_ ## func ## _5, 4, 6)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_5_ov,                \
        py_ ## func ## _task_5, 5, 7)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_5_DEF_2(rt, target, func, p1, p2, p3, p4, v4,  \
        p5, v5)                                                               \
    DEFINE_PYTHON_WRAPPERS_5_DEF_2_EX(rt, target, func, func, p1, p2, p3,     \
        p4, v4, p5, v5)                                                       \
    /**/

#define DEFINE_PYTHON_WRAPPERS_5_DEF_3_EX(rt, target, func, sfunc, p1, p2,    \
        p3, v3, p4, v4, p5, v5)                                               \
    static rt py_ ## func ## _5(target o, p1 par1, p2 par2, p3 par3 = v3,     \
        p4 par4 = v4, p5 par5 = v5)                                           \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4, par5);                         \
    }                                                                         \
    static saga::task py_ ## func ## _task_5(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2, p3 par3 = v3,      \
        p4 par4 = v4, p5 par5 = v5)                                           \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4, par5);\
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2, par3, par4, par5);\
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2, par3, par4, par5);\
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _5_ov,                     \
        py_ ## func ## _5, 3, 6)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_5_ov,                \
        py_ ## func ## _task_5, 4, 7)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_5_DEF_3(rt, target, func, p1, p2, p3, v3,      \
        p4, v4, p5, v5)                                                       \
    DEFINE_PYTHON_WRAPPERS_5_DEF_3_EX(rt, target, func, func, p1, p2, p3, v3, \
        p4, v4, p5, v5)                                                       \
    /**/

#define DEFINE_PYTHON_WRAPPERS_5_DEF_4_EX(rt, target, func, sfunc, p1, p2, v2,\
        p3, v3, p4, v4, p5, v5)                                               \
    static rt py_ ## func ## _5(target o, p1 par1, p2 par2 = v2,              \
        p3 par3 = v3, p4 par4 = v4, p5 par5 = v5)                             \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4, par5);                         \
    }                                                                         \
    static saga::task py_ ## func ## _task_5(target o,                        \
        saga::python::routine_type type, p1 par1, p2 par2 = v2, p3 par3 = v3, \
        p4 par4 = v4, p5 par5 = v5)                                           \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4, par5);\
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2, par3, par4, par5);\
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2, par3, par4, par5);\
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _5_ov,                     \
        py_ ## func ## _5, 2, 6)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_5_ov,                \
        py_ ## func ## _task_5, 3, 7)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_5_DEF_4(rt, target, func, p1, p2, v2, p3, v3,  \
        p4, v4, p5, v5)                                                       \
    DEFINE_PYTHON_WRAPPERS_5_DEF_4_EX(rt, target, func, func, p1, p2, v2,     \
        p3, v3, p4, v4, p5, v5)                                               \
    /**/

#define DEFINE_PYTHON_WRAPPERS_5_DEF_5_EX(rt, target, func, sfunc, p1, v1,    \
        p2, v2, p3, v3, p4, v4, p5, v5)                                       \
    static rt py_ ## func ## _5(target o, p1 par1 = v1, p2 par2 = v2,         \
        p3 par3 = v3, p4 par4 = v4, p5 par5 = v5)                             \
    {                                                                         \
        return o.sfunc(par1, par2, par3, par4, par5);                         \
    }                                                                         \
    static saga::task py_ ## func ## _task_5(target o,                        \
        saga::python::routine_type type, p1 par1 = v1, p2 par2 = v2,          \
        p3 par3 = v3, p4 par4 = v4, p5 par5 = v5)                             \
    {                                                                         \
        switch(type) {                                                        \
        case saga::python::Sync:                                              \
            return o.sfunc<saga::task_base::Sync>(par1, par2, par3, par4, par5); \
        case saga::python::Async:                                             \
            return o.sfunc<saga::task_base::Async>(par1, par2, par3, par4, par5);\
        case saga::python::Task:                                              \
            return o.sfunc<saga::task_base::Task>(par1, par2, par3, par4, par5); \
        default:                                                              \
            SAGA_THROW_VERBATIM(o,                                            \
                "Invalid routine type (not Sync, Async or Task)",             \
                saga::BadParameter);                                          \
            break;                                                            \
        }                                                                     \
        return saga::task();                                                  \
    }                                                                         \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _5_ov,                     \
        py_ ## func ## _5, 1, 6)                                              \
    BOOST_PYTHON_FUNCTION_OVERLOADS(py_ ## func ## _task_5_ov,                \
        py_ ## func ## _task_5, 2, 7)                                         \
    /**/
#define DEFINE_PYTHON_WRAPPERS_5_DEF_5(rt, target, func, p1, v1, p2, v2,      \
        p3, v3, p4, v4, p5, v5)                                               \
    DEFINE_PYTHON_WRAPPERS_5_DEF_4_EX(rt, target, func, func, p1, v1, p2, v2, \
        p3, v3, p4, v4, p5, v5)                                               \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_5_EX(func, sfunc, doc)                          \
    .def(#sfunc, py_ ## func ## _5, doc " (plain)")                           \
    .def(#sfunc, py_ ## func ## _task_5, doc " (task based)")                 \
    /**/    
#define INSERT_PYTHON_WRAPPER_5(func, doc)                                    \
    INSERT_PYTHON_WRAPPER_5_EX(func, func, doc)                               \
    /**/

///////////////////////////////////////////////////////////////////////////////
#define INSERT_PYTHON_WRAPPER_5_EX_OVERLOADS(func, sfunc, doc)                \
    .def(#sfunc, py_ ## func ## _5, py_ ## func ## _5_ov(doc " (plain)"))     \
    .def(#sfunc, py_ ## func ## _task_5, py_ ## func ## _task_5_ov(           \
        doc " (task based)"))                                                 \
    /**/
#define INSERT_PYTHON_WRAPPER_5_OVERLOADS(func, doc)                          \
    INSERT_PYTHON_WRAPPER_5_EX_OVERLOADS(func, func, doc)                     \
    /**/

#endif // PY_VERSION_HEX < 0x0204000

#endif // !PYTHON_ROUTINE_TYPES_HPP

