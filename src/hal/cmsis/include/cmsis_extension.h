/*
 * This file is part of the OpenMV project.
 *
 * Copyright (c) 2013-2024 Ibrahim Abdelkader <iabdalkader@openmv.io>
 * Copyright (c) 2013-2024 Kwabena W. Agyeman <kwagyeman@openmv.io>
 *
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * CMSIS Extension
 */
#ifndef __CMSIS_EXTENSION_H
#define __CMSIS_EXTENSION_H
#include "cmsis_compiler.h"

#if ((defined (__ARM_ARCH_7M__      ) && (__ARM_ARCH_7M__      == 1)) || \
     (defined (__ARM_ARCH_7EM__     ) && (__ARM_ARCH_7EM__     == 1)) || \
     (defined (__ARM_ARCH_8M_MAIN__ ) && (__ARM_ARCH_8M_MAIN__ == 1))    )

/**
  \brief   Signed Saturate
  \details Saturates a signed value.
  \param [in]  ARG1  Value to be saturated
  \param [in]  ARG2  Bit position to saturate to (1..32)
  \param [in]  ARG3  Right shift (0..31)
  \return             Saturated value
 */
#define __SSAT_ASR(ARG1, ARG2, ARG3) \
__extension__ \
({                          \
  int32_t __RES, __ARG1 = (ARG1); \
  __ASM volatile ("ssat %0, %1, %2, asr %3" : "=r" (__RES) :  "I" (ARG2), "r" (__ARG1), "I" (ARG3) : "cc" ); \
  __RES; \
 })

/**
  \brief   Unsigned Saturate
  \details Saturates an unsigned value.
  \param [in]  ARG1  Value to be saturated
  \param [in]  ARG2  Bit position to saturate to (0..31)
  \param [in]  ARG3  Right shift (0..31)
  \return             Saturated value
 */
#define __USAT_ASR(ARG1, ARG2, ARG3) \
 __extension__ \
({                          \
  uint32_t __RES, __ARG1 = (ARG1); \
  __ASM volatile ("usat %0, %1, %2, asr %3" : "=r" (__RES) :  "I" (ARG2), "r" (__ARG1), "I" (ARG3) : "cc" ); \
  __RES; \
 })

#else  /* ((defined (__ARM_ARCH_7M__      ) && (__ARM_ARCH_7M__      == 1)) || \
           (defined (__ARM_ARCH_7EM__     ) && (__ARM_ARCH_7EM__     == 1)) || \
           (defined (__ARM_ARCH_8M_MAIN__ ) && (__ARM_ARCH_8M_MAIN__ == 1))    ) */

/**
  \brief   Signed Saturate
  \details Saturates a signed value.
  \param [in]  value  Value to be saturated
  \param [in]    sat  Bit position to saturate to (1..32)
  \param [in]  shift  Right shift (0..31)
  \return             Saturated value
 */
__STATIC_FORCEINLINE int32_t __SSAT_ASR(int32_t val, uint32_t sat, uint32_t shift)
{
  val >>= shift & 0x1F;

  if ((sat >= 1U) && (sat <= 32U))
  {
    const int32_t max = (int32_t)((1U << (sat - 1U)) - 1U);
    const int32_t min = -1 - max ;
    if (val > max)
    {
      return max;
    }
    else if (val < min)
    {
      return min;
    }
  }
  return val;
}

/**
  \brief   Signed Saturate
  \details Saturates two signed values.
  \param [in]  value  Values to be saturated
  \param [in]    sat  Bit position to saturate to (1..16)
  \return             Saturated value
 */
__STATIC_FORCEINLINE int32_t __SSAT16(int32_t val, uint32_t sat)
{
  if ((sat >= 1U) && (sat <= 32U))
  {
    const int32_t max = (int32_t)((1U << (sat - 1U)) - 1U);
    const int32_t min = -1 - max ;
    int32_t valHi = val >> 16;
    if (valHi > max)
    {
      valHi = max;
    }
    else if (valHi < min)
    {
      valHi = min;
    }
    int32_t valLo = (val << 16) >> 16;
    if (valLo > max)
    {
      valLo = max;
    }
    else if (valLo < min)
    {
      valLo = min;
    }
    return (valHi << 16) | (valLo & 0xFFFF);
  }
  return val;
}

/**
  \brief   Unsigned Saturate
  \details Saturates an unsigned value.
  \param [in]  value  Value to be saturated
  \param [in]    sat  Bit position to saturate to (0..31)
  \param [in]  shift  Right shift (0..31)
  \return             Saturated value
 */
__STATIC_FORCEINLINE uint32_t __USAT_ASR(int32_t val, uint32_t sat, uint32_t shift)
{
  val >>= shift & 0x1F;

  if (sat <= 31U)
  {
    const uint32_t max = ((1U << sat) - 1U);
    if (val > (int32_t)max)
    {
      return max;
    }
    else if (val < 0)
    {
      return 0U;
    }
  }
  return (uint32_t)val;
}

/**
  \brief   Unsigned Saturate
  \details Saturates two unsigned values.
  \param [in]  value  Values to be saturated
  \param [in]    sat  Bit position to saturate to (0..15)
  \return             Saturated value
 */
__STATIC_FORCEINLINE uint32_t __USAT16(int32_t val, uint32_t sat)
{
  if (sat <= 15U)
  {
    const uint32_t max = ((1U << sat) - 1U);
    int32_t valHi = val >> 16;
    if (valHi > (int32_t)max)
    {
      valHi = max;
    }
    else if (valHi < 0)
    {
      valHi = 0U;
    }
    int32_t valLo = (val << 16) >> 16;
    if (valLo > (int32_t)max)
    {
      valLo = max;
    }
    else if (valLo < 0)
    {
      valLo = 0U;
    }
    return (valHi << 16) | valLo;
  }
  return (uint32_t)val;
}

#endif /* ((defined (__ARM_ARCH_7M__      ) && (__ARM_ARCH_7M__      == 1)) || \
           (defined (__ARM_ARCH_7EM__     ) && (__ARM_ARCH_7EM__     == 1)) || \
           (defined (__ARM_ARCH_8M_MAIN__ ) && (__ARM_ARCH_8M_MAIN__ == 1))    ) */

#if (defined (__ARM_FEATURE_DSP) && (__ARM_FEATURE_DSP == 1))

__STATIC_FORCEINLINE uint32_t __UXTB(uint32_t op1)
{
  uint32_t result;

  __ASM volatile ("uxtb %0, %1" : "=r" (result) : "r" (op1));
  return(result);
}

__STATIC_FORCEINLINE uint32_t __UXTB_RORn(uint32_t op1, uint32_t rotate)
{
  uint32_t result;

  __ASM volatile ("uxtb %0, %1, ROR %2" : "=r" (result) : "r" (op1), "i" (rotate) );
  return result;
}

__STATIC_FORCEINLINE uint32_t __UXTB16_RORn(uint32_t op1, uint32_t rotate)
{
  uint32_t result;

  __ASM volatile ("uxtb16 %0, %1, ROR %2" : "=r" (result) : "r" (op1), "i" (rotate) );
  return result;
}

__STATIC_FORCEINLINE uint32_t __UXTAB_RORn(uint32_t op1, uint32_t op2, uint32_t rotate)
{
  uint32_t result;

  __ASM volatile ("uxtab %0, %1, %2, ROR %3" : "=r" (result) : "r" (op1), "r" (op2), "i" (rotate) );
  return result;
}

__STATIC_FORCEINLINE uint32_t __SXTB(uint32_t op1)
{
  uint32_t result;

  __ASM volatile ("sxtb %0, %1" : "=r" (result) : "r" (op1));
  return(result);
}

__STATIC_FORCEINLINE uint32_t __SXTB_RORn(uint32_t op1, uint32_t rotate)
{
  uint32_t result;

  __ASM volatile ("sxtb %0, %1, ROR %2" : "=r" (result) : "r" (op1), "i" (rotate) );
  return result;
}

#else

__STATIC_FORCEINLINE uint32_t __UXTB(uint32_t op1)
{
  return op1 & 0xFF;
}

__STATIC_FORCEINLINE uint32_t __UXTB_RORn(uint32_t op1, uint32_t rotate)
{
  return (op1 >> rotate) & 0xFF;
}

__STATIC_FORCEINLINE uint32_t __SSUB16(uint32_t op1, uint32_t op2)
{
  return ((op1 & 0xFFFF0000) - (op2 & 0xFFFF0000)) | ((op1 - op2) & 0xFFFF);
}

#endif /* (__ARM_FEATURE_DSP == 1) */
#endif /* __CMSIS_EXTENSIONS_H */
