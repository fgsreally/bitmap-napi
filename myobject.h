#include <node_api.h>
#include "boost/dynamic_bitset.hpp"
class Bitset
{
public:
    static napi_value Init(napi_env env, napi_value exports);
    static void Destructor(napi_env env, void *nativeObject, void *finalize_hint);

private:
    explicit Bitset(uint32_t pos);
    ~Bitset();
    static napi_value New(napi_env env, napi_callback_info info);
    static napi_value GetBitPos(napi_env env, napi_callback_info info);
    static napi_value SetBitPos(napi_env env, napi_callback_info info);
    static napi_value CancelBitPos(napi_env env, napi_callback_info info);
     static napi_value Value(napi_env env, napi_callback_info info);
     static napi_ref constructor;   
    napi_env env_;
    napi_ref wrapper_;
    boost::dynamic_bitset<> myBitset;
    uint32_t length_;
};
