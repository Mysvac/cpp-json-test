#pragma once



#define M_REGISTER_CLASS(lib_name, class_name) \
    static void register_##class_name () { \
        jtu::TestClassFactory::instance().register_class<class_name>(lib_name); \
    } \
    static bool registered_##class_name = ( register_##class_name () , true);



