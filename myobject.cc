#include "myobject.h"
#include <assert.h>

napi_ref Bitset::constructor;

Bitset::Bitset(uint32_t pos)
    : env_(nullptr), wrapper_(nullptr), myBitset(boost::dynamic_bitset<>(pos)), length_(pos) {}

Bitset::~Bitset() { napi_delete_reference(env_, wrapper_); }

void Bitset::Destructor(napi_env env, void *nativeObject, void *)
{
    reinterpret_cast<Bitset *>(nativeObject)->~Bitset();
}

#define DECLARE_NAPI_METHOD(name, func)         \
    {                                           \
        name, 0, func, 0, 0, 0, napi_default, 0 \
    }

napi_value Bitset::Init(napi_env env, napi_value exports)
{
    napi_property_descriptor properties[] = {
        DECLARE_NAPI_METHOD("getPos", GetBitPos),
        DECLARE_NAPI_METHOD("setPos", SetBitPos),
        DECLARE_NAPI_METHOD("cancelPos", CancelBitPos),
        DECLARE_NAPI_METHOD("value", Value),
    };
    napi_value cons;
    napi_define_class(env, "Bitset", NAPI_AUTO_LENGTH, New, nullptr, 4, properties, &cons);
    napi_create_reference(env, cons, 1, &constructor);
    napi_set_named_property(env, exports, "Bitset", cons);
    return exports;
}
napi_value Bitset::New(napi_env env, napi_callback_info info)
{
    napi_value target;
    napi_get_new_target(env, info, &target);
    napi_value jsthis;
    napi_status status;
    status = napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr);
    assert(status == napi_ok);
    size_t argc = 1; //规定argc的字节数为2
    napi_value args;
    status = napi_get_cb_info(env, info, &argc, &args, NULL, NULL);
    assert(status == napi_ok);
    //参数个数不能小于2
    if (argc > 1)
    {
        napi_throw_type_error(env, NULL, "Wrong number of arguments");
        return NULL;
    }
    //输入参数类型判断，status的值是指那个方法是否正常调用，正常输出
    napi_valuetype valuetype0;
    status = napi_typeof(env, args, &valuetype0);
    assert(status == napi_ok);

    if (valuetype0 != napi_number)
    {
        napi_throw_type_error(env, NULL, "Wrong arguments");
        return NULL;
    }
    uint32_t value0;
    status = napi_get_value_uint32(env, args, &value0);
    assert(status == napi_ok);
    Bitset *obj = new Bitset(value0);
    obj->env_ = env;
    napi_wrap(env,
              jsthis,
              reinterpret_cast<void *>(obj),
              Bitset::Destructor,
              nullptr,
              &obj->wrapper_);
    return jsthis;
}

napi_value Bitset::GetBitPos(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value arg;
    napi_value jsthis;
    napi_get_cb_info(env, info, &argc, &arg, &jsthis, nullptr);
    Bitset *obj;
    napi_unwrap(env, jsthis, reinterpret_cast<void **>(&obj));
    uint32_t pos;
    napi_get_value_uint32(env, arg, &pos);
    napi_value result;
    uint32_t bitValue = (obj->myBitset)[pos];
    napi_create_uint32(env, bitValue, &result);
    return result;
}
napi_value Bitset::SetBitPos(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value arg;
    napi_value jsthis;
    napi_get_cb_info(env, info, &argc, &arg, &jsthis, nullptr);
    uint32_t pos;
    napi_get_value_uint32(env, arg, &pos);
    Bitset *obj;
    napi_unwrap(env, jsthis, reinterpret_cast<void **>(&obj));
    (obj->myBitset)[pos] = 1;
    return nullptr;
}
napi_value Bitset::CancelBitPos(napi_env env, napi_callback_info info)
{
    size_t argc = 1;
    napi_value arg;
    napi_value jsthis;
    napi_get_cb_info(env, info, &argc, &arg, &jsthis, nullptr);
    uint32_t pos;
    napi_get_value_uint32(env, arg, &pos);
    Bitset *obj;
    napi_unwrap(env, jsthis, reinterpret_cast<void **>(&obj));
    (obj->myBitset)[pos] = 0;
    return nullptr;
}

napi_value Bitset::Value(napi_env env, napi_callback_info info)
{
    napi_value jsthis;
    napi_get_cb_info(env, info, nullptr, nullptr, &jsthis, nullptr);
    Bitset *obj;
    napi_unwrap(env, jsthis, reinterpret_cast<void **>(&obj));
    napi_value result;
    napi_create_array(env, &result);
    uint32_t length = (obj->length_);
    napi_value uintjs;
    for (uint32_t i = 0; i < length; i++)
    {
        uint32_t bitValue = (obj->myBitset)[i];
        napi_create_uint32(env, bitValue, &uintjs);
        // result[i]=uintjs;
        napi_set_element(env,
                         result,
                         i,
                         uintjs);
    }
    return result;
}