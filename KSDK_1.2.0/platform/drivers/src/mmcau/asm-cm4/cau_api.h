/*
 *  CAU Header File
 *  Works with library cau_lib.a and lib_mmcau*.a
 *  Define FREESCALE_CAU if CAU coprocessor is used --Register parameter passing is assumed
 *  Define FREESCALE_MMCAU if mmCAU coprocessor is used --EABI for Kinetis ARM Cortex-Mx
 *  12/19/2013
 */

#if FREESCALE_MMCAU
#define cau_aes_set_key               mmcau_aes_set_key
#define cau_aes_encrypt               mmcau_aes_encrypt
#define cau_aes_decrypt               mmcau_aes_decrypt
#define cau_des_chk_parity            mmcau_des_chk_parity
#define cau_des_encrypt               mmcau_des_encrypt
#define cau_des_decrypt               mmcau_des_decrypt
#define cau_md5_initialize_output     mmcau_md5_initialize_output
#define cau_md5_hash_n                mmcau_md5_hash_n
#define cau_md5_update                mmcau_md5_update
#define cau_md5_hash                  mmcau_md5_hash
#define cau_sha1_initialize_output    mmcau_sha1_initialize_output
#define cau_sha1_hash_n               mmcau_sha1_hash_n
#define cau_sha1_update               mmcau_sha1_update
#define cau_sha1_hash                 mmcau_sha1_hash
#define cau_sha256_initialize_output  mmcau_sha256_initialize_output
#define cau_sha256_hash_n             mmcau_sha256_hash_n
#define cau_sha256_update             mmcau_sha256_update
#define cau_sha256_hash               mmcau_sha256_hash
#endif

//******************************************************************************
// 
// AES: Performs an AES key expansion
//   arguments
//          *key        pointer to input key (128, 192, 256 bits in length)
//           key_size   key_size in bits (128, 192, 256)
//          *key_sch    pointer to key schedule output (44, 52, 60 longwords)
//
//   calling convention
//   void   cau_aes_set_key (const unsigned char *key,
//                           const int            key_size,
//                           unsigned char       *key_sch)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
void 
cau_aes_set_key (const unsigned char *key, const int key_size, 
                 unsigned char *key_sch);

//******************************************************************************
//******************************************************************************
//
// AES: Encrypts a single 16-byte block
//   arguments
//          *in         pointer to 16-byte block of input plaintext
//          *key_sch    pointer to key schedule (44, 52, 60 longwords)
//           nr         number of AES rounds (10, 12, 14 = f(key_schedule))
//          *out        pointer to 16-byte block of output ciphertext
//
//   NOTE   Input and output blocks may overlap
//
//   calling convention
//   void   cau_aes_encrypt (const unsigned char *in,
//                           const unsigned char *key_sch,
//                           const int            nr,
//                           unsigned char       *out)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
void 
cau_aes_encrypt (const unsigned char *in, const unsigned char *key_sch, 
                 const int nr, unsigned char *out);

//******************************************************************************
//******************************************************************************
//
// AES: Decrypts a single 16-byte block
//   arguments
//          *in         pointer to 16-byte block of input chiphertext
//          *key_sch    pointer to key schedule (44, 52, 60 longwords)
//           nr         number of AES rounds (10, 12, 14 = f(key_schedule))
//          *out        pointer to 16-byte block of output plaintext
//
//   NOTE   Input and output blocks may overlap
//
//   calling convention
//   void   cau_aes_decrypt (const unsigned char *in,
//                           const unsigned char *key_sch,
//                           const int            nr,
//                           unsigned char       *out)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
void 
cau_aes_decrypt (const unsigned char *in, const unsigned char *key_sch, 
                 const int nr, unsigned char *out);
    
//******************************************************************************
// 
// DES: Checks key parity
//   arguments
//          *key        pointer to 64-bit DES key with parity bits
//
//   return
//          0           no error
//         -1           parity error
//
//   calling convention
//   int    cau_des_chk_parity (const unsigned char *key)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
int 
cau_des_chk_parity (const unsigned char *key);

//******************************************************************************
//
// DES: Encrypts a single 8-byte block
//   arguments
//          *in         pointer to 8-byte block of input plaintext
//          *key        pointer to 64-bit DES key with parity bits
//          *out        pointer to 8-byte block of output ciphertext
//
//   NOTE   Input and output blocks may overlap
//
//   calling convention
//   void     cau_des_encrypt (const unsigned char *in,
//                             const unsigned char *key,
//                             unsigned char       *out)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
void 
cau_des_encrypt (const unsigned char *in, const unsigned char *key, 
                 unsigned char *out);

//******************************************************************************
// 
// DES: Decrypts a single 8-byte block
//   arguments
//          *in         pointer to 8-byte block of input ciphertext
//          *key        pointer to 64-bit DES key with parity bits
//          *out        pointer to 8-byte block of output plaintext
//   
//   NOTE   Input and output blocks may overlap
//   
//   calling convention
//   void   cau_des_decrypt (const unsigned char *in,
//                           const unsigned char *key,
//                           unsigned char       *out)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
void 
cau_des_decrypt (const unsigned char *in, const unsigned char *key, 
                 unsigned char *out);
    
//******************************************************************************
//******************************************************************************
//
// MD5: Initializes the MD5 state variables
//   arguments
//          *md_state   pointer to 120-bit block of md5 state variables:
//                      a,b,c,d
//
//   calling convention
//   void   cau_md5_initialize_output (const unsigned char *md_state)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
void 
cau_md5_initialize_output (const unsigned char *md5_state);

//******************************************************************************
//******************************************************************************
//
// MD5: Updates MD5 state variables for one or more input message blocks
//
//   arguments
//          *msg_data   pointer to start of input message data
//           num_blks   number of 512-bit blocks to process
//          *md_state   pointer to 128-bit block of MD5 state variables:
//                      a,b,c,d
//
//   calling convention
//   void   cau_md5_hash_n (const unsigned char *msg_data,
//                          const int            num_blks,
//                          unsigned char       *md_state)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
void 
cau_md5_hash_n (const unsigned char *msg_data, const int num_blks, 
                unsigned char *md5_state);

//******************************************************************************
//******************************************************************************
//
// MD5: Updates MD5 state variables for one or more input message blocks
//   arguments
//          *msg_data   pointer to start of input message data
//           num_blks   number of 512-bit blocks to process
//          *md_state   pointer to 128-bit block of MD5 state variables: 
//                      a,b,c,d
//
//   calling convention
//   void   cau_md5_update (const unsigned char *msg_data,
//                          const int            num_blks,
//                          unsigned char       *md_state)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
void 
cau_md5_update (const unsigned char *msg_data, const int num_blks, 
                unsigned char *md5_state);

//******************************************************************************
//******************************************************************************
//
// MD5: Performs MD5 hash algorithm for a single input message block
//          *msg_data   pointer to start of input message data
//          *md_state   pointer to 128-bit block of MD5 state variables:
//                      a,b,c,d
//
//   calling convention
//   void   cau_md5_hash (const unsigned char *msg_data,
//                        unsigned char       *md_state)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
void 
cau_md5_hash (const unsigned char *msg_data, unsigned char *md5_state);

//******************************************************************************
//******************************************************************************
//
// SHA1: Initializes the SHA1 state variables
//   arguments
//          *sha1_state pointer to 160-bit block of SHA1 state variables:
//                      a,b,c,d,e
//
//   calling convention
//   void   cau_sha1_initialize_output (const unsigned int *sha1_state)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
void 
cau_sha1_initialize_output (const unsigned int *sha1_state);

//******************************************************************************
//******************************************************************************
// 
// SHA1: Perform the hash and generate SHA1 state variables for one or more 
//       input message blocks 
//  arguments
//          *msg_data   pointer to start of input message data
//           num_blks   number of 512-bit blocks to process
//          *sha1_state pointer to 160-bit block of SHA1 state variables:
//                      a,b,c,d,e
//
//   NOTE   Input message and state variable output blocks must not overlap
//
//   calling convention
//   void   cau_sha1_hash (const unsigned char *msg_data,
//                         const int            num_blks,
//                         unsigned int        *sha1_state)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
void 
cau_sha1_hash_n (const unsigned char *msg_data, const int num_blks, 
                 unsigned int *sha1_state);

//******************************************************************************
//******************************************************************************
// 
// SHA1: Updates SHA1 state variables for one or more input message blocks
//   arguments
//          *msg_data   pointer to start of input message data
//           num_blks   number of 512-bit blocks to process
//          *sha1_state pointer to 160-bit block of SHA1 state variables:
//                      a,b,c,d,e
//
//   calling convention
//   void   cau_sha1_update (const unsigned char *msg_data,
//                           const int            num_blks,
//                           unsigned int        *sha1_state)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
void 
cau_sha1_update (const unsigned char *msg_data, const int num_blks, 
                 unsigned int *sha1_state);

//******************************************************************************
//******************************************************************************
// 
// SHA1: Performs SHA1 hash algorithm on a single input message block
//   arguments
//          *msg_data   pointer to start of input message data
//          *sha1_state pointer to 160-bit block of SHA1 state variables:
//                      a,b,c,d,e
//
//   calling convention
//   void   cau_sha1_update (const unsigned char *msg_data,
//                           unsigned int        *sha1_state)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
void 
cau_sha1_hash (const unsigned char *msg_data, 
               unsigned int *sha1_state);
    
//******************************************************************************
//******************************************************************************
// 
// SHA256: Initializes the hash output and checks the CAU hardware revision
//   arguments
//          *output     pointer to 256-bit message digest output
//
//   return
//          0           no error -> CAU2 hardware present
//         -1           error -> incorrect CAU hardware revision
//
//   calling convention
//   int    cau_sha256_initialize_output (const unsigned int *output)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
int 
cau_sha256_initialize_output (const unsigned int *output);

//******************************************************************************
//******************************************************************************
// 
// SHA256: Updates SHA256 digest output for one or more message block arguments 
//   arguments
//          *input      pointer to start of input message
//           input      number of 512-bit blocks to process
//          *output     pointer to 256-bit message digest output
// 
//   NOTE   Input message and digest output blocks must not overlap
// 
//   calling convention
//   void   cau_sha256_hash_n (const unsigned char *input,
//                             int                  num_blks,
//                             const unsigned int  *output)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
void 
cau_sha256_hash_n (const unsigned char *input, const int num_blks, 
                   unsigned int *output);

//******************************************************************************
//******************************************************************************
//
// SHA256: Updates SHA256 state variables for one or more input message blocks
//   arguments
//          *input      pointer to start of input message data
//           num_blks   number of 512-bit blocks to process
//          *output     pointer to 256-bit message digest output
//
//   calling convention
//   void   cau_sha256_update (const unsigned char *input,
//                             const int            num_blks,
//                             unsigned int        *output)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
void 
cau_sha256_update (const unsigned char *input, const int num_blks, 
                   unsigned int *output);

//******************************************************************************
//******************************************************************************
//
// SHA256: Performs SHA256 hash algorithm for a single input message block
//   arguments
//          *input      pointer to start of input message data
//          *output     pointer to 256-bit message digest output
//
//   calling convention
//   void   cau_sha256_hash (const unsigned char *input,
//                           unsigned int        *output)
#if FREESCALE_CAU
__declspec (standard_abi)
#endif
void 
cau_sha256_hash (const unsigned char *input, unsigned int *output);
