#pragma once

namespace XSystem {
    namespace Encryption
    {
        class XXTea
        {
        public:

            enum class Type : int
            {
                Decrypt,
                Encrypt,
            };

            static bool Excute(Type type, unsigned int* a3, int valueSize)
            {
                int j;
                int shift;
                unsigned round = 6;
                unsigned int tmp;
                unsigned int v;
                unsigned long key[4] = { 
                    0x173456A8,
                    0xB34A67FC,
                    0x3C67A642,
                    0x1E432526,
                };

                if (valueSize <= 1)
                    return false;

                if (type == Type::Encrypt)
                {
                    tmp = 0;
                    v = a3[valueSize - 1];
                    while (round-- > 0)
                    {
                        tmp -= 0x61C88647;
                        shift = (tmp >> 2) & 3;
                        for (j = 0; j < valueSize - 1; ++j)
                        {
                            a3[j] += ((v^ key[shift ^ j & 3]) + (a3[j + 1] ^ tmp)) ^ (((16 * v) ^ (a3[j + 1] >> 3)) + ((4 * a3[j + 1]) ^ (v >> 5)));
                            v = a3[j];
                        }
                        a3[valueSize - 1] += ((v ^ key[shift ^ j & 3]) + (a3[0] ^ tmp)) ^ (((16 * v) ^ (a3[0] >> 3)) + ((4 * a3[0]) ^ (v >> 5)));
                    }
                }
                else
                {
                    tmp = a3[0];
                    v = 0xB54CDA56;
                    while (round-- > 0)
                    {
                        shift = (v >> 2) & 3;
                        for (j = valueSize - 1; j > 0; --j)
                        {
                            a3[j] -= ((a3[j - 1] ^ key[shift ^ j & 3]) + (tmp ^ v)) ^ (((16 * a3[j - 1]) ^ (tmp >> 3)) + ((4 * tmp) ^ (a3[j - 1] >> 5)));
                            tmp = a3[j];
                        }
                        a3[0] -= ((a3[valueSize - 1] ^ key[shift ^ j & 3]) + (tmp ^ v)) ^ (((16 * a3[valueSize - 1]) ^ (tmp >> 3)) + ((4 * tmp) ^ (a3[valueSize - 1] >> 5)));
                        tmp = a3[0];
                        v += 0x61C88647;
                    }
                }
                return true;
            }
        };
    }
}
