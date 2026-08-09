#include "nvs_manager.h"

#include "nvs.h"
#include "nvs_flash.h"

app_status_t nvs_manager_init()
{
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ret = nvs_flash_erase();

        if (ret != ESP_OK)
        {
            return APP_ERROR;
        }

        ret = nvs_flash_init();

        if (ret != ESP_OK)
        {
            return APP_ERROR;
        }
    }
    else if (ret != ESP_OK)
    {
        return APP_ERROR;
    }

    return APP_OK;

}

app_status_t nvs_manager_write_string(
    const char *namespace_name,
    const char *key,
    const char *value)
{
    nvs_handle_t handle;

    esp_err_t ret = nvs_open(
        namespace_name,
        NVS_READWRITE,
        &handle
    );

    if (ret != ESP_OK)
    {
        return APP_ERROR;
    }

    ret = nvs_set_str(handle, key, value);

    if (ret != ESP_OK)
    {
        nvs_close(handle);
        return APP_ERROR;
    }

    ret = nvs_commit(handle);

    nvs_close(handle);

    if (ret != ESP_OK)
    {
        return APP_ERROR;
    }

    return APP_OK;
}

app_status_t nvs_manager_read_string(
    const char *namespace_name,
    const char *key,
    char *value,
    size_t value_size)
{
    nvs_handle_t handle;

    esp_err_t ret = nvs_open(
        namespace_name,
        NVS_READONLY,
        &handle
    );

    if (ret != ESP_OK)
    {
        return ret;
    }

    ret = nvs_get_str(
        handle,
        key,
        value,
        &value_size
    );

    nvs_close(handle);

    if (ret != ESP_OK)
    {
        return ret;
    }

    return APP_OK;
}

app_status_t nvs_manager_erase_namespace(const char *namespace_name)
{
    nvs_handle_t handle;

    esp_err_t ret = nvs_open(
        namespace_name,
        NVS_READWRITE,
        &handle
    );

    if (ret != ESP_OK)
    {
        return APP_ERROR;
    }

    ret = nvs_erase_all(handle);

    if (ret == ESP_OK)
    {
        ret = nvs_commit(handle);
    }

    nvs_close(handle);

    if (ret != ESP_OK)
    {
        return APP_ERROR;
    }

    return APP_OK;
}