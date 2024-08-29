#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <string>
#include <iostream>

// Helper function to Base64 encode a string
std::string base64_encode(const std::string &in) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;
    std::string out;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);
    BIO_write(bio, in.data(), in.size());
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    out.assign(bufferPtr->data, bufferPtr->length - 1); // remove the newline character
    BIO_free_all(bio);
    return out;
}

// Helper function to calculate SHA-1 hash
std::string sha1(const std::string &input) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);
    return std::string(reinterpret_cast<char*>(hash), SHA_DIGEST_LENGTH);
}

// Function to generate Sec-WebSocket-Accept value
std::string generate_websocket_accept_key(const std::string &key) {
    const std::string GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    std::string key_with_guid = key + GUID;

    // Calculate SHA-1 hash of the concatenated key and GUID
    std::string sha1_hash = sha1(key_with_guid);

    // Base64 encode the SHA-1 hash
    return base64_encode(sha1_hash);
}

int main() {
    std::string secWebSocketKey = "dGhlIHNhbXBsZSBub25jZQ=="; // Example client key
    std::string acceptKey = generate_websocket_accept_key(secWebSocketKey);
    std::cout << "Sec-WebSocket-Accept: " << acceptKey << std::endl;
    return 0;
}
