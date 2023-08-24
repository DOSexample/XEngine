#pragma once

namespace XSystem {
    namespace Encryption
    {
        const int TEA_ROUND = 6;
        const unsigned int TEA_DELTA = 0x61C88647;
        const unsigned int TEA_SUM = 0xB54CDA56;

        class XXTea
        {

        public:

            enum class Type : int
            {
                Decrypt,
                Encrypt,
            };

            static bool Excute(Type type, unsigned int* data, int len)
            {
                //keys
                unsigned int k[4] = {
                    0x173456A8,
                    0xB34A67FC,
                    0x3C67A642,
                    0x1E432526
                };

                if (len <= 1)
                    return false;

                if (type == Type::Encrypt)
                {
                    tea1_encrypt(data, len, k, TEA_DELTA);
                }
                else
                {
                    tea1_decrypt(data, len, k, TEA_DELTA);
                }
                return true;
            }

            static void tea1_encrypt(unsigned int* v, int len, const unsigned int* k, const unsigned int delta)
            {
                int n = TEA_ROUND, j, shift;
                uint32_t sum = 0;
                uint32_t cur;

                while (n-- > 0)
                {
                    cur = v[len - 1];
                    sum -= delta;
                    shift = (sum >> 2) & 3;
                    for (j = 0; j < len - 1; ++j)
                    {
                        v[j] += ((cur ^ k[shift ^ j & 3]) + (v[j + 1] ^ sum)) ^ (((16 * cur) ^ (v[j + 1] >> 3)) + ((4 * v[j + 1]) ^ (cur >> 5)));
                        cur = v[j];
                    }
                    v[len - 1] += ((cur ^ k[shift ^ j & 3]) + (v[0] ^ sum)) ^ (((16 * cur) ^ (v[0] >> 3)) + ((4 * v[0]) ^ (cur >> 5)));
                }
            }

            static void tea1_decrypt(unsigned int* v, int len, const unsigned int* k, const unsigned int delta)
            {
                int n = TEA_ROUND, j, shift;
                uint32_t sum;
                uint32_t cur;

                sum = TEA_SUM;
                while (n-- > 0)
                {
                    cur = v[0];
                    shift = (sum >> 2) & 3;
                    for (j = len - 1; j > 0; --j)
                    {
                        v[j] -= ((v[j - 1] ^ k[shift ^ j & 3]) + (cur ^ sum)) ^ (((16 * v[j - 1]) ^ (cur >> 3)) + ((4 * cur) ^ (v[j - 1] >> 5)));
                        cur = v[j];
                    }
                    v[0] -= ((v[len - 1] ^ k[shift ^ j & 3]) + (cur ^ sum)) ^ (((16 * v[len - 1]) ^ (cur >> 3)) + ((4 * cur) ^ (v[len - 1] >> 5)));
                    sum += delta;
                }
            }
        };
    }
}
