/*
 * Copyright (C) 2020-2021 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "ColorSpace.h"
#include <CoreGraphics/CoreGraphics.h>
#include <optional>
#include <wtf/cf/TypeCastsCF.h>

WTF_DECLARE_CF_TYPE_TRAIT(CGColorSpace);

namespace WebCore {

WEBCORE_EXPORT CGColorSpaceRef sRGBColorSpaceRef();

#if HAVE(CORE_GRAPHICS_ADOBE_RGB_1998_COLOR_SPACE)
WEBCORE_EXPORT CGColorSpaceRef adobeRGB1998ColorSpaceRef();
#endif

#if HAVE(CORE_GRAPHICS_DISPLAY_P3_COLOR_SPACE)
WEBCORE_EXPORT CGColorSpaceRef displayP3ColorSpaceRef();
#endif

#if HAVE(CORE_GRAPHICS_EXTENDED_ADOBE_RGB_1998_COLOR_SPACE)
WEBCORE_EXPORT CGColorSpaceRef extendedAdobeRGB1998ColorSpaceRef();
#endif

#if HAVE(CORE_GRAPHICS_EXTENDED_DISPLAY_P3_COLOR_SPACE)
WEBCORE_EXPORT CGColorSpaceRef extendedDisplayP3ColorSpaceRef();
#endif

#if HAVE(CORE_GRAPHICS_EXTENDED_ITUR_2020_COLOR_SPACE)
WEBCORE_EXPORT CGColorSpaceRef extendedITUR_2020ColorSpaceRef();
#endif

#if HAVE(CORE_GRAPHICS_EXTENDED_LINEAR_SRGB_COLOR_SPACE)
WEBCORE_EXPORT CGColorSpaceRef extendedLinearSRGBColorSpaceRef();
#endif

#if HAVE(CORE_GRAPHICS_EXTENDED_ROMMRGB_COLOR_SPACE)
WEBCORE_EXPORT CGColorSpaceRef extendedROMMRGBColorSpaceRef();
#endif

#if HAVE(CORE_GRAPHICS_EXTENDED_SRGB_COLOR_SPACE)
WEBCORE_EXPORT CGColorSpaceRef extendedSRGBColorSpaceRef();
#endif

#if HAVE(CORE_GRAPHICS_ITUR_2020_COLOR_SPACE)
WEBCORE_EXPORT CGColorSpaceRef ITUR_2020ColorSpaceRef();
#endif

#if HAVE(CORE_GRAPHICS_LINEAR_SRGB_COLOR_SPACE)
WEBCORE_EXPORT CGColorSpaceRef linearSRGBColorSpaceRef();
#endif

#if HAVE(CORE_GRAPHICS_ROMMRGB_COLOR_SPACE)
WEBCORE_EXPORT CGColorSpaceRef ROMMRGBColorSpaceRef();
#endif

#if HAVE(CORE_GRAPHICS_XYZ_COLOR_SPACE)
WEBCORE_EXPORT CGColorSpaceRef xyzD50ColorSpaceRef();
#endif

std::optional<ColorSpace> colorSpaceForCGColorSpace(CGColorSpaceRef);

static inline CGColorSpaceRef cachedNullableCGColorSpace(ColorSpace colorSpace)
{
    switch (colorSpace) {
    case ColorSpace::SRGB:
        return sRGBColorSpaceRef();

    case ColorSpace::A98RGB:
#if HAVE(CORE_GRAPHICS_ADOBE_RGB_1998_COLOR_SPACE)
        return adobeRGB1998ColorSpaceRef();
#else
        return nullptr;
#endif

    case ColorSpace::DisplayP3:
#if HAVE(CORE_GRAPHICS_DISPLAY_P3_COLOR_SPACE)
        return displayP3ColorSpaceRef();
#else
        return nullptr;
#endif

    case ColorSpace::ExtendedA98RGB:
#if HAVE(CORE_GRAPHICS_EXTENDED_ADOBE_RGB_1998_COLOR_SPACE)
        return extendedAdobeRGB1998ColorSpaceRef();
#else
        return nullptr;
#endif

    case ColorSpace::ExtendedDisplayP3:
#if HAVE(CORE_GRAPHICS_EXTENDED_DISPLAY_P3_COLOR_SPACE)
        return extendedDisplayP3ColorSpaceRef();
#else
        return nullptr;
#endif

    case ColorSpace::ExtendedLinearSRGB:
#if HAVE(CORE_GRAPHICS_EXTENDED_LINEAR_SRGB_COLOR_SPACE)
        return extendedLinearSRGBColorSpaceRef();
#else
        return nullptr;
#endif

    case ColorSpace::ExtendedProPhotoRGB:
#if HAVE(CORE_GRAPHICS_EXTENDED_ROMMRGB_COLOR_SPACE)
        return extendedROMMRGBColorSpaceRef();
#else
        return nullptr;
#endif

    case ColorSpace::ExtendedRec2020:
#if HAVE(CORE_GRAPHICS_EXTENDED_ITUR_2020_COLOR_SPACE)
        return extendedITUR_2020ColorSpaceRef();
#else
        return nullptr;
#endif

    case ColorSpace::ExtendedSRGB:
#if HAVE(CORE_GRAPHICS_EXTENDED_SRGB_COLOR_SPACE)
        return extendedSRGBColorSpaceRef();
#else
        return nullptr;
#endif

    case ColorSpace::LinearSRGB:
#if HAVE(CORE_GRAPHICS_LINEAR_SRGB_COLOR_SPACE)
        return linearSRGBColorSpaceRef();
#else
        return nullptr;
#endif

    case ColorSpace::ProPhotoRGB:
#if HAVE(CORE_GRAPHICS_ROMMRGB_COLOR_SPACE)
        return ROMMRGBColorSpaceRef();
#else
        return nullptr;
#endif

    case ColorSpace::Rec2020:
#if HAVE(CORE_GRAPHICS_ITUR_2020_COLOR_SPACE)
        return ITUR_2020ColorSpaceRef();
#else
        return nullptr;
#endif

    case ColorSpace::XYZ_D50:
#if HAVE(CORE_GRAPHICS_XYZ_COLOR_SPACE)
        return xyzD50ColorSpaceRef();
#else
        return nullptr;
#endif

    // FIXME: Add support for these once/if CoreGraphics adds support for it.
    case ColorSpace::HSL:
    case ColorSpace::HWB:
    case ColorSpace::LCH:
    case ColorSpace::Lab:
    case ColorSpace::OKLCH:
    case ColorSpace::OKLab:
    case ColorSpace::XYZ_D65:
        return nullptr;


    }
    ASSERT_NOT_REACHED();
    return nullptr;
}

}
