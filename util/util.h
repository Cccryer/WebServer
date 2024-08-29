#ifndef UTIL_H
#define UTIL_H
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <string>

//编译添加-lssl -lcrypto

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

#endif