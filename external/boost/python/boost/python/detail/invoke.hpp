#if !defined(BOOST_PP_IS_ITERATING)

// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
# ifndef INVOKE_DWA20021122_HPP
#  define INVOKE_DWA20021122_HPP

#  include <boost/python/detail/prefix.hpp>
#  include <boost/python/detail/preprocessor.hpp>
#  include <boost/python/detail/none.hpp>

#  include <boost/type_traits/is_member_function_pointer.hpp>

#  include <boost/preprocessor/iterate.hpp>
#  include <boost/preprocessor/facilities/intercept.hpp>
#  include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#  include <boost/preprocessor/repetition/enum_trailing_binary_params.hpp>
#  include <boost/preprocessor/repetition/enum_binary_params.hpp>
#  include <boost/python/to_python_value.hpp>

#include <boost/call_traits.hpp>

// Predeclaration of py_iter_
namespace boost { namespace python { namespace objects { 
    template <class NextPolicies, class Iterator> struct iterator_range;
    namespace detail {
        template<class Target, class Iterator, class Accessor1, class Accessor2, class NextPolicies> struct py_iter_;
  }
}}}

// This file declares a series of overloaded invoke(...)  functions,
// used to invoke wrapped C++ function (object)s from Python. Each one
// accepts:
//
//   - a tag which identifies the invocation syntax (e.g. member
//   functions must be invoked with a different syntax from regular
//   functions)
//
//   - a pointer to a result converter type, used solely as a way of
//   transmitting the type of the result converter to the function (or
//   an int, if the return type is void).
//
//   - the "function", which may be a function object, a function or
//   member function pointer, or a defaulted_virtual_fn.
//
//   - The arg_from_python converters for each of the arguments to be
//   passed to the function being invoked.

namespace boost { namespace python { namespace detail { 

// This "result converter" is really just used as a dispatch tag to
// invoke(...), selecting the appropriate implementation
typedef int void_result_to_python;

// ned 23rd Oct 2003: invoke modifications based on those of Lijun Qin, thanks to him!
template <bool void_return, bool member>
struct invoke_tag_ {};

// A meta-function returning the appropriate tag type for invoking an
// object of type F with return type R.
template <class R, class F>
struct invoke_tag
  : invoke_tag_<
        is_same<R,void>::value
      , is_member_function_pointer<F>::value
    >
{
};

// forward declaration of member && datum
template<class Data, class Class> struct member;
template <class Data> struct datum;

// forward declaration of return extractor
template<class F> struct ReturnExtract;
// Specialization for member data & datum data
template<class Data, class Class> struct ReturnExtract<boost::python::detail::member<Data, Class> >
{
    typedef Data value;
    enum { GIL=true };
};
template<class Data> struct ReturnExtract<boost::python::detail::datum<Data> >
{
    typedef Data value;
    enum { GIL=true };
};
// Specialization for iterators
template<class Target, class Iterator, class Accessor1, class Accessor2, class NextPolicies>
struct ReturnExtract<boost::python::objects::detail::py_iter_<Target, Iterator, Accessor1, Accessor2, NextPolicies> >
{
    typedef boost::python::objects::iterator_range<NextPolicies, Iterator> value;
    enum { GIL=false };
};
// Fix for boost::python::objects::iterator_range<>::next (no other way of doing it :( )
template <class T> struct ReturnExtract
{
    typedef typename T::result_type value;
    enum { GIL=false };
};

// Placeholder for do_cpp when it's disabled
struct do_cpp_dummy
{
    void undo() const { }
};

#if PY_VERSION_HEX >= 0x02040000
///////////////////////////////////////////////////////////////////////////////
// #HK: added wrappers for switching between C++ and Python
struct do_cpp
{
    do_cpp() : done_(false), save_(NULL) 
        { redo(); }
    ~do_cpp() 
        { undo(); }

    void redo()
    { 
        if (!done_ && PyEval_ThreadsInitialized()) { 
            save_ = PyEval_SaveThread(); 
            done_ = true; 
        } 
    }
    void undo()
    { 
        if (done_ && PyEval_ThreadsInitialized()) { 
            PyEval_RestoreThread(save_); 
            save_ = NULL; 
            done_ = false;
        }
    }

    bool done_;
    PyThreadState *save_;
};

///////////////////////////////////////////////////////////////////////////////
struct do_python
{
    do_python() : done_(false), state_(PyGILState_UNLOCKED) 
        { redo(); }
    ~do_python() 
        { undo(); }

    void redo()
    { 
        if (!done_ && PyEval_ThreadsInitialized()) { 
            state_ = PyGILState_Ensure(); 
            done_ = true; 
        } 
    }
    void undo()
    { 
        if (done_ && PyEval_ThreadsInitialized()) { 
            PyGILState_Release(state_); 
            state_ = PyGILState_UNLOCKED; 
            done_ = false;
        }
    }

    bool done_;
    PyGILState_STATE state_;
};
#else
struct do_cpp
{
      do_cpp() {}
      ~do_cpp() {}

      void redo() {}
      void undo() {}
};

///////////////////////////////////////////////////////////////////////////////
struct do_python
{
      do_python() {}
      ~do_python() {}

      void redo() {}
      void undo() {}
};
#endif

///////////////////////////////////////////////////////////////////////////////
#  define BOOST_PP_ITERATION_PARAMS_1                                         \
        (3, (0, BOOST_PYTHON_MAX_ARITY, <boost/python/detail/invoke.hpp>))
#  include BOOST_PP_ITERATE()

}}} // namespace boost::python::detail

# endif // INVOKE_DWA20021122_HPP
#else 

# define N BOOST_PP_ITERATION()

#define CONVERT_ARG(z, n, _) typename boost::call_traits<typename AC##n::result_type>::param_type a##n = ac##n();

template<typename R, typename O BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class AC)>
struct ReturnExtract<R (O::*)(BOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, AC, ac) )>
{
    typedef R value;
    enum { GIL=true };
};

// template<typename R, typename O BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class AC)>
// struct ReturnExtract<R (O::*)(BOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, AC, ac) ) throw()>
// {
//     typedef R value;
//     enum { GIL=true };
// };

template<typename R, typename O BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class AC)>
struct ReturnExtract<R (O::*)(BOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, AC, ac) ) const>
{
    typedef R value;
    enum { GIL=true };
};

// template<typename R, typename O BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class AC)>
// struct ReturnExtract<R (O::*)(BOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, AC, ac) ) const throw()>
// {
//     typedef R value;
//     enum { GIL=true };
// };

template<typename R BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class AC)>
struct ReturnExtract<R (*)(BOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, AC, ac) )>
{
    typedef R value;
    enum { GIL=true };
};

// template<typename R BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class AC)>
// struct ReturnExtract<R (*)(BOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, AC, ac) ) throw()>
// {
//     typedef R value;
//     enum { GIL=true };
// };

///////////////////////////////////////////////////////////////////////////////
template <class RC, class F BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class AC)>
inline PyObject* invoke(invoke_tag_<false,false>, RC const& rc, F& f BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(1, N, AC, & ac) )
{
    BOOST_PP_REPEAT(N, CONVERT_ARG,    nil)
    typedef ReturnExtract<F> RetExt;
    typedef typename RetExt::value rettype;
    typename boost::mpl::if_c<RetExt::GIL, do_cpp, do_cpp_dummy>::type cpphold;        // Releases the Python GIL
    rettype ret=f(BOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, a, BOOST_PP_INTERCEPT));
    cpphold.undo();
    return rc(ret);
    //return rc(f( BOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, ac, () BOOST_PP_INTERCEPT) ));
}
                 
template <class RC, class F BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class AC)>
inline PyObject* invoke(invoke_tag_<true,false>, RC const&, F& f BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(1, N, AC, & ac) )
{
    BOOST_PP_REPEAT(N, CONVERT_ARG,    nil)
    do_cpp cpphold;       // Releases the Python GIL
    f(BOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, a, BOOST_PP_INTERCEPT));
    cpphold.undo();
    //f( BOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, ac, () BOOST_PP_INTERCEPT) );
    return none();
}

template <class RC, class F, class TC BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class AC)>
inline PyObject* invoke(invoke_tag_<false,true>, RC const& rc, F& f, TC& tc BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(1, N, AC, & ac) )
{
    BOOST_PP_REPEAT(N, CONVERT_ARG,    nil)
    typename TC::result_type c=tc();
    typedef ReturnExtract<F> RetExt;
    typedef typename RetExt::value rettype;
    typename boost::mpl::if_c<RetExt::GIL, do_cpp, do_cpp_dummy>::type cpphold;        // Releases the Python GIL
    rettype ret=(c.*f)(BOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, a, BOOST_PP_INTERCEPT));
    cpphold.undo();
    return rc(ret);
    //return rc( (tc().*f)(BOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, ac, () BOOST_PP_INTERCEPT)) );
}
                 
template <class RC, class F, class TC BOOST_PP_ENUM_TRAILING_PARAMS_Z(1, N, class AC)>
inline PyObject* invoke(invoke_tag_<true,true>, RC const&, F& f, TC& tc BOOST_PP_ENUM_TRAILING_BINARY_PARAMS_Z(1, N, AC, & ac) )
{
    BOOST_PP_REPEAT(N, CONVERT_ARG,    nil)
    typename TC::result_type c=tc();
    do_cpp cpphold;        // Releases the Python GIL
    (c.*f)(BOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, a, BOOST_PP_INTERCEPT));
    cpphold.undo();
    //(tc().*f)(BOOST_PP_ENUM_BINARY_PARAMS_Z(1, N, ac, () BOOST_PP_INTERCEPT));
    return none();
}

# undef N

#endif // BOOST_PP_IS_ITERATING 
