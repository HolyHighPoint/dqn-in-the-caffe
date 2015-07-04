#pragma once

#ifdef _MSC_VER
#   define QLEARNING_API_EXPORT_DECLARATION __declspec(dllexport)
#   define QLEARNING_API_IMPORT_DECLARATION __declspec(dllimport)
#elif __GNUC__
#	define QLEARNING_API_EXPORT_DECLARATION __attribute__ ((visibility ("default")))
#	define QLEARNING_API_IMPORT_DECLARATION __attribute__ ((visibility ("default")))
#else
#   define QLEARNING_API_EXPORT_DECLARATION
#   define QLEARNING_API_IMPORT_DECLARATION
#endif 

#ifndef QLEARNING_STATIC
#ifdef QLEARNING_EXPORTS
#   define QLEARNING_API QLEARNING_API_EXPORT_DECLARATION
#else
#   define QLEARNING_API QLEARNING_API_IMPORT_DECLARATION
#endif
#else
#   define QLEARNING_API
#endif

#ifdef N_DEBUG
#   define IF_DEBUG(statement)
#   define IF_NDEBUG(statement) statement
#else
#   define IF_DEBUG(statement) statement
#   define IF_NDEBUG(statement)
#endif // N_DEBUG

#ifndef NOEXCEPT
#   ifdef _MSC_VER
#       define NOEXCEPT
#   else
#       define NOEXCEPT noexcept
#   endif
#endif


/**
 * \namespace mstlib
 *
 * \brief Contains all the core classes that ... functionality.
 */
namespace qlearning {

};
