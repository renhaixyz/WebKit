/*
 * Copyright (C) 2021 Apple Inc. All rights reserved.
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

#include "config.h"
#include "InlineDisplayLineBuilder.h"

#if ENABLE(LAYOUT_FORMATTING_CONTEXT)

#include "LayoutBoxGeometry.h"

namespace WebCore {
namespace Layout {

InlineDisplayLineBuilder::InlineDisplayLineBuilder(const InlineFormattingContext& inlineFormattingContext)
    : m_inlineFormattingContext(inlineFormattingContext)
{
}

InlineDisplayLineBuilder::EnclosingLineGeometry InlineDisplayLineBuilder::collectEnclosingLineGeometry(const LineBox& lineBox, const InlineRect& lineBoxRect) const
{
    auto& rootInlineBox = lineBox.rootInlineBox();
    auto scrollableOverflowRect = lineBoxRect;
    auto enclosingTopAndBottom = InlineDisplay::Line::EnclosingTopAndBottom { lineBoxRect.top() + rootInlineBox.logicalTop(), lineBoxRect.top() + rootInlineBox.logicalBottom() };

    for (auto& inlineLevelBox : lineBox.nonRootInlineLevelBoxes()) {
        if (!inlineLevelBox.isAtomicInlineLevelBox() && !inlineLevelBox.isInlineBox())
            continue;

        auto& layoutBox = inlineLevelBox.layoutBox();
        auto borderBox = InlineRect { };

        if (inlineLevelBox.isAtomicInlineLevelBox()) {
            borderBox = lineBox.logicalBorderBoxForAtomicInlineLevelBox(layoutBox, formattingContext().geometryForBox(layoutBox));
            borderBox.moveBy(lineBoxRect.topLeft());
        } else if (inlineLevelBox.isInlineBox()) {
            auto& boxGeometry = formattingContext().geometryForBox(layoutBox);
            borderBox = lineBox.logicalBorderBoxForInlineBox(layoutBox, boxGeometry);
            borderBox.moveBy(lineBoxRect.topLeft());
            // Collect scrollable overflow from inline boxes. All other inline level boxes (e.g atomic inline level boxes) stretch the line.
            auto hasScrollableContent = [&] {
                // In standards mode, inline boxes always start with an imaginary strut.
                return layoutState().inStandardsMode() || inlineLevelBox.hasContent() || boxGeometry.horizontalBorder() || (boxGeometry.horizontalPadding() && boxGeometry.horizontalPadding().value());
            };
            if (lineBox.hasContent() && hasScrollableContent()) {
                // Empty lines (e.g. continuation pre/post blocks) don't expect scrollbar overflow.
                scrollableOverflowRect.expandToContain(borderBox);
            }
        } else
            ASSERT_NOT_REACHED();

        enclosingTopAndBottom.top = std::min(enclosingTopAndBottom.top, borderBox.top());
        enclosingTopAndBottom.bottom = std::max(enclosingTopAndBottom.bottom, borderBox.bottom());
    }
    return { enclosingTopAndBottom, scrollableOverflowRect };
}

InlineDisplay::Line InlineDisplayLineBuilder::build(const LineBuilder::LineContent& lineContent, const LineBox& lineBox, InlineLayoutUnit lineBoxLogicalHeight, size_t lineIndex) const
{
    auto& rootStyle = lineIndex ? root().firstLineStyle() : root().style();
    auto& rootInlineBox = lineBox.rootInlineBox();
    auto& rootGeometry = layoutState().geometryForBox(root());
    auto isLeftToRightDirection = rootStyle.isLeftToRightDirection();
    auto lineOffsetFromContentBox = lineContent.lineLogicalTopLeft.x() - rootGeometry.contentBoxLeft();

    auto lineBoxVisualLeft = isLeftToRightDirection
        ? rootGeometry.contentBoxLeft() + lineOffsetFromContentBox
        : InlineLayoutUnit { rootGeometry.borderEnd() } + rootGeometry.paddingEnd().value_or(0_lu);
    auto contentVisualLeft = isLeftToRightDirection
        ? lineBox.rootInlineBoxAlignmentOffset()
        : rootGeometry.contentBoxWidth() - lineOffsetFromContentBox -  lineBox.rootInlineBoxAlignmentOffset() - rootInlineBox.logicalWidth() - lineContent.hangingContentWidth;

    auto lineBoxRect = InlineRect { lineContent.lineLogicalTopLeft.y(), lineBoxVisualLeft, lineContent.lineLogicalWidth, lineBoxLogicalHeight };
    auto enclosingLineGeometry = collectEnclosingLineGeometry(lineBox, lineBoxRect);

    return InlineDisplay::Line { lineBoxRect
        , enclosingLineGeometry.scrollableOverflowRect
        , enclosingLineGeometry.enclosingTopAndBottom
        , rootInlineBox.logicalTop() + rootInlineBox.baseline()
        , contentVisualLeft
        , rootInlineBox.logicalWidth()
    };
}

}
}

#endif
