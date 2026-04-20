#include <stdio.h>
#include <zenoh-pico.h>

static int g_received = 0;

static void data_handler(z_loaned_sample_t *sample, void *ctx) {
    (void)ctx;

    z_view_string_t keystr;
    z_keyexpr_as_view_string(z_sample_keyexpr(sample), &keystr);

    z_owned_string_t value;
    z_bytes_to_string(z_sample_payload(sample), &value);

    printf("Received: ('%.*s': '%.*s')\n", (int)z_string_len(z_loan(keystr)), z_string_data(z_loan(keystr)),
           (int)z_string_len(z_loan(value)), z_string_data(z_loan(value)));

    z_drop(z_move(value));
    g_received++;
}

int main(void) {
    const char *connect_endpoint = "tcp/127.0.0.1:7447";
    const char *keyexpr = "demo/example/zenoh-pico-simple-pubsub";
    const char *value = "Hello from zenoh-pico!";
    const int publish_count = 10;

    // ----- key expression -----
    // Create key expression for publisher and subscriber
    z_view_keyexpr_t ke;
    if (z_view_keyexpr_from_str(&ke, keyexpr) < 0) {
        printf("Invalid key expression: %s\n", keyexpr);
        return -1;
    }

    // ----- subscriber -----
    z_owned_config_t sub_config;
    z_config_default(&sub_config);
    zp_config_insert(z_loan_mut(sub_config), Z_CONFIG_CONNECT_KEY, connect_endpoint);

    z_owned_session_t sub_session;
    if (z_open(&sub_session, z_move(sub_config), NULL) < 0) {
        printf("Unable to open subscriber session.\n");
        return -1;
    }

    z_owned_closure_sample_t callback;
    z_closure(&callback, data_handler, NULL, NULL);

    z_owned_subscriber_t subscriber;
    if (z_declare_subscriber(z_loan(sub_session), &subscriber, z_loan(ke), z_move(callback), NULL) < 0) {
        printf("Unable to declare subscriber.\n");
        z_drop(z_move(sub_session));
        return -1;
    }

    // ----- publisher -----
    z_owned_config_t pub_config;
    z_config_default(&pub_config);
    zp_config_insert(z_loan_mut(pub_config), Z_CONFIG_CONNECT_KEY, connect_endpoint);

    z_owned_session_t pub_session;
    if (z_open(&pub_session, z_move(pub_config), NULL) < 0) {
        printf("Unable to open publisher session.\n");
        z_drop(z_move(subscriber));
        z_drop(z_move(sub_session));
        return -1;
    }

    z_owned_publisher_t publisher;
    if (z_declare_publisher(z_loan(pub_session), &publisher, z_loan(ke), NULL) < 0) {
        printf("Unable to declare publisher.\n");
        z_drop(z_move(subscriber));
        z_drop(z_move(pub_session));
        z_drop(z_move(sub_session));
        return -1;
    }

    // Give zenohd a brief moment to register the subscriber before publishing.
    z_sleep_s(1);

    for (int idx = 0; idx < publish_count; ++idx) {
        char buf[128];
        snprintf(buf, sizeof(buf), "[%d] %s", idx, value);

        z_owned_bytes_t payload;
        z_bytes_copy_from_str(&payload, buf);
        z_publisher_put(z_loan(publisher), z_move(payload), NULL);
        printf("Published: %s\n", buf);
        z_sleep_s(1);
    }

    // Wait a little for the routed samples to arrive, but do not hang forever if
    // the router is unavailable or the subscriber did not match in time.
    int wait_loops = 0;
    while (g_received < publish_count && wait_loops < 50) {
        z_sleep_ms(100);
        wait_loops++;
    }

    if (g_received != publish_count) {
        printf("Only received %d/%d samples.\n", g_received, publish_count);
    }

    z_drop(z_move(publisher));
    z_drop(z_move(subscriber));
    z_drop(z_move(pub_session));
    z_drop(z_move(sub_session));
    return 0;
}
