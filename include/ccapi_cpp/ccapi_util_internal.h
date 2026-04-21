#pragma once

// Internal-only utilities used by ccapi's own translation units (service/*.h,
// ccapi_session.cpp, etc). Do NOT include this header from any public header
// that user code transitively reaches - it pulls in OpenSSL, <regex>,
// <fstream>, which inflate user-TU compile time.

#include <array>
#include <fstream>
#include <numeric>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "openssl/evp.h"
#include "openssl/pem.h"

#include "ccapi_cpp/ccapi_util.h"  // light utilities

namespace ccapi {

class UtilAlgorithm {
 public:
  enum class ShaVersion {
    UNKNOWN,
    SHA256,
    SHA512,
  };

 public:
  static std::string toBase62(size_t value) {
    static constexpr char kBase62Chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::string result;
    do {
      result += kBase62Chars[value % 62];
      value /= 62;
    } while (value);
    std::reverse(result.begin(), result.end());
    return result;
  }

  static std::string shortBase62Hash(const std::string& input) {
    std::hash<std::string> hasher;
    return toBase62(hasher(input));
  }

  static std::string computeHash(const ShaVersion shaVersion, const std::string& unhashed, bool returnHex = false) {
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    switch (shaVersion) {
      case ShaVersion::SHA256:
        EVP_DigestInit_ex(context, EVP_sha256(), NULL);
        break;
      case ShaVersion::SHA512:
        EVP_DigestInit_ex(context, EVP_sha512(), NULL);
        break;
      default:
        EVP_MD_CTX_free(context);
        throw std::invalid_argument("invalid shaVersion");
    }
    EVP_DigestUpdate(context, unhashed.c_str(), unhashed.length());
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;
    EVP_DigestFinal_ex(context, hash, &lengthOfHash);
    EVP_MD_CTX_free(context);
    std::stringstream ss;
    if (returnHex) {
      for (unsigned int i = 0; i < lengthOfHash; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
      }
    } else {
      for (unsigned int i = 0; i < lengthOfHash; ++i) {
        ss << (char)hash[i];
      }
    }
    return ss.str();
  }

  static std::string stringToHex(const std::string& input) {
    static const char hex_digits[] = "0123456789abcdef";
    std::string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input) {
      output.push_back(hex_digits[c >> 4]);
      output.push_back(hex_digits[c & 15]);
    }
    return output;
  }

  static int hexValue(unsigned char hex_digit) {
    static const signed char hex_values[256] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    };
    int value = hex_values[hex_digit];
    if (value == -1) throw std::invalid_argument("invalid hex digit");
    return value;
  }

  static std::string hexToString(const std::string& input) {
    const auto len = input.length();
    if (len & 1) throw std::invalid_argument("odd length");
    std::string output;
    output.reserve(len / 2);
    for (auto it = input.begin(); it != input.end();) {
      int hi = hexValue(*it++);
      int lo = hexValue(*it++);
      output.push_back(hi << 4 | lo);
    }
    return output;
  }

  // https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
  static std::string base64Encode(const std::string& input) {
    static const unsigned char base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const unsigned char* src = reinterpret_cast<const unsigned char*>(input.c_str());
    size_t len = input.length();
    unsigned char *out, *pos;
    const unsigned char *end, *in;
    size_t olen;
    olen = 4 * ((len + 2) / 3);
    if (olen < len) return std::string();
    std::string outStr;
    outStr.resize(olen);
    out = (unsigned char*)&outStr[0];
    end = src + len;
    in = src;
    pos = out;
    while (end - in >= 3) {
      *pos++ = base64_table[in[0] >> 2];
      *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
      *pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
      *pos++ = base64_table[in[2] & 0x3f];
      in += 3;
    }
    if (end - in) {
      *pos++ = base64_table[in[0] >> 2];
      if (end - in == 1) {
        *pos++ = base64_table[(in[0] & 0x03) << 4];
        *pos++ = '=';
      } else {
        *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        *pos++ = base64_table[(in[1] & 0x0f) << 2];
      }
      *pos++ = '=';
    }
    return outStr;
  }

  static std::string base64Decode(const std::string& in) {
    static const int B64index[256] = {0, 0, 0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,
                                      0, 0, 0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  62, 63, 62, 62, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0,  0,  0, 0,
                                      0, 0, 0,  0, 1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0,
                                      0, 0, 63, 0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};
    const void* data = in.c_str();
    const size_t len = in.length();
    unsigned char* p = (unsigned char*)data;
    int pad = len > 0 && (len % 4 || p[len - 1] == '=');
    const size_t L = ((len + 3) / 4 - pad) * 4;
    std::string str(L / 4 * 3 + pad, '\0');

    for (size_t i = 0, j = 0; i < L; i += 4) {
      int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
      str[j++] = n >> 16;
      str[j++] = n >> 8 & 0xFF;
      str[j++] = n & 0xFF;
    }
    if (pad) {
      int n = B64index[p[L]] << 18 | B64index[p[L + 1]] << 12;
      str[str.size() - 1] = n >> 16;

      if (len > L + 2 && p[L + 2] != '=') {
        n |= B64index[p[L + 2]] << 6;
        str.push_back(n >> 8 & 0xFF);
      }
    }
    return str;
  }

  //  https://github.com/brianloveswords/base64url
  static std::string base64UrlFromBase64(const std::string& base64) {
    return std::regex_replace(std::regex_replace(std::regex_replace(base64, std::regex("="), ""), std::regex("\\+"), "-"), std::regex("\\/"), "_");
  }

  static std::string base64FromBase64Url(const std::string& base64Url) {
    auto segmentLength = 4;
    auto stringLength = base64Url.size();
    auto diff = stringLength % segmentLength;
    if (!diff) {
      return base64Url;
    }
    auto padLength = segmentLength - diff;
    std::string paddedBase64Url(base64Url);
    paddedBase64Url += std::string(padLength, '=');
    return std::regex_replace(std::regex_replace(paddedBase64Url, std::regex("\\-"), "+"), std::regex("_"), "/");
  }

  static std::string base64UrlEncode(const std::string& in) { return base64UrlFromBase64(base64Encode(in)); }

  static std::string base64UrlDecode(const std::string& in) { return base64Decode(base64FromBase64Url(in)); }

  static double exponentialBackoff(double initial, double multiplier, double base, double exponent) { return initial + multiplier * (pow(base, exponent) - 1); }

  template <typename InputIterator>
  static uint_fast32_t crc(InputIterator first, InputIterator last);

  static EVP_PKEY* loadPrivateKey(const std::string& keyPem, const std::string& password = "") {
    BIO* bio = BIO_new_mem_buf(keyPem.data(), static_cast<int>(keyPem.size()));
    if (!bio) return nullptr;

    const EVP_PKEY_ASN1_METHOD* meth = EVP_PKEY_asn1_find_str(nullptr, "ED25519", -1);
    if (!meth) {
      throw std::runtime_error("ED25519 is NOT supported in this OpenSSL build.");
    }

    EVP_PKEY* pkey = nullptr;
    if (password.empty()) {
      pkey = PEM_read_bio_PrivateKey(bio, nullptr, nullptr, nullptr);
    } else {
      pkey = PEM_read_bio_PrivateKey(bio, nullptr, nullptr, const_cast<char*>(password.c_str()));
    }

    BIO_free(bio);
    return pkey;
  }

  static std::string readFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open file: " + path);
    }

    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
  }

  static std::string base64Encode(const std::vector<unsigned char>& input) {
    BIO *bio, *b64;
    BUF_MEM* bufferPtr;
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bio);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(b64, input.data(), static_cast<int>(input.size()));
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bufferPtr);
    std::string result(bufferPtr->data, bufferPtr->length);
    BIO_free_all(b64);
    return result;
  }

  static std::string signPayload(EVP_PKEY* pkey, const std::string& payload) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) throw std::runtime_error("Failed to create EVP_MD_CTX");

    const int key_type = EVP_PKEY_base_id(pkey);
    const bool is_ed25519 = key_type == EVP_PKEY_ED25519;

    if (EVP_DigestSignInit(ctx, nullptr, is_ed25519 ? nullptr : EVP_sha256(), nullptr, pkey) != 1) throw std::runtime_error("EVP_DigestSignInit failed");

    size_t sigLen = 0;

    if (is_ed25519) {
      if (EVP_DigestSign(ctx, nullptr, &sigLen, reinterpret_cast<const unsigned char*>(payload.data()), payload.size()) != 1)
        throw std::runtime_error("EVP_DigestSign (get length) failed");

      std::vector<unsigned char> signature(sigLen);
      if (EVP_DigestSign(ctx, signature.data(), &sigLen, reinterpret_cast<const unsigned char*>(payload.data()), payload.size()) != 1)
        throw std::runtime_error("EVP_DigestSign failed");
      signature.resize(sigLen);
      EVP_MD_CTX_free(ctx);
      return base64Encode(signature);
    } else {
      if (EVP_DigestSignUpdate(ctx, payload.data(), payload.size()) != 1) throw std::runtime_error("EVP_DigestSignUpdate failed");

      if (EVP_DigestSignFinal(ctx, nullptr, &sigLen) != 1) throw std::runtime_error("EVP_DigestSignFinal (get length) failed");

      std::vector<unsigned char> signature(sigLen);
      if (EVP_DigestSignFinal(ctx, signature.data(), &sigLen) != 1) throw std::runtime_error("EVP_DigestSignFinal failed");

      signature.resize(sigLen);
      EVP_MD_CTX_free(ctx);
      return base64Encode(signature);
    }
  }

  static const EVP_MD* getDigest(const ShaVersion version) {
    switch (version) {
      case ShaVersion::SHA256:
        return EVP_sha256();
      case ShaVersion::SHA512:
        return EVP_sha512();
      default:
        throw std::invalid_argument("Unsupported SHA version");
    }
  }
};

template <typename InputIterator>
inline uint_fast32_t UtilAlgorithm::crc(InputIterator first, InputIterator last) {
  static auto const table = []() {
    auto const reversed_polynomial = uint_fast32_t{0xEDB88320uL};
    struct byte_checksum {
      uint_fast32_t operator()() noexcept {
        auto checksum = static_cast<uint_fast32_t>(n++);
        for (auto i = 0; i < 8; ++i) checksum = (checksum >> 1) ^ ((checksum & 0x1u) ? reversed_polynomial : 0);
        return checksum;
      }
      unsigned n = 0;
    };
    auto table = std::array<uint_fast32_t, 256>{};
    std::generate(table.begin(), table.end(), byte_checksum{});
    return table;
  }();
  return uint_fast32_t{0xFFFFFFFFuL} &
         ~std::accumulate(first, last, ~uint_fast32_t{0} & uint_fast32_t{0xFFFFFFFFuL},
                          [](uint_fast32_t checksum, std::uint_fast8_t value) { return table[(checksum ^ value) & 0xFFu] ^ (checksum >> 8); });
}

}  // namespace ccapi
