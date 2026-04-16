#include <stdio.h>
#include <zenoh-pico.h>

int main(void) {
    const char *connect_endpoint = "tcp/127.0.0.1:7447";
    const char *keyexpr = "demo/example/zenoh-pico-simple-advanced-pub";
    const char *value = "Hello from advanced zenoh-pico!";

    z_owned_config_t config;
    z_config_default(&config);
    zp_config_insert(z_loan_mut(config), Z_CONFIG_CONNECT_KEY, connect_endpoint);

    z_owned_session_t session;
    if (z_open(&session, z_move(config), NULL) < 0) {
        printf("Unable to open session.\n");
        return -1;
    }

    z_view_keyexpr_t ke;
    if (z_view_keyexpr_from_str(&ke, keyexpr) < 0) {
        printf("Invalid key expression: %s\n", keyexpr);
        z_drop(z_move(session));
        return -1;
    }

    ze_advanced_publisher_options_t options;
    ze_advanced_publisher_options_default(&options);
    options.cache.is_enabled = true;
    options.cache.max_samples = 1;

    ze_owned_advanced_publisher_t publisher;
    if (ze_declare_advanced_publisher(z_loan(session), &publisher, z_loan(ke), &options) < 0) {
        printf("Unable to declare advanced publisher.\n");
        z_drop(z_move(session));
        return -1;
    }

    for (int idx = 0; idx < 10; ++idx) {
        char buf[128];
        snprintf(buf, sizeof(buf), "[%d] %s", idx, value);

        z_owned_bytes_t payload;
        z_bytes_copy_from_str(&payload, buf);
        ze_advanced_publisher_put(z_loan(publisher), z_move(payload), NULL);
        printf("Published: %s\n", buf);
        z_sleep_s(1);
    }

    z_drop(z_move(publisher));
    z_drop(z_move(session));
    return 0;
}
