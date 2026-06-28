//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Markdown.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/SmallVector.h"

namespace clang::doc::markdown {

//===----------------------------------------------------------------------===//
// Inline node print/dump
//===----------------------------------------------------------------------===//

LLVM_DUMP_METHOD void InlineNode::dump() const { print(llvm::errs()); }

void TextNode::print(llvm::raw_ostream &OS) const {
  OS << "TextNode: " << getText() << "\n";
}

void InlineCodeNode::print(llvm::raw_ostream &OS) const {
  OS << "InlineCodeNode: " << getCode() << "\n";
}

void EmphasisNode::print(llvm::raw_ostream &OS) const {
  OS << "EmphasisNode\n";
  for (const auto &Child : Children)
    Child.print(OS);
}

void StrongNode::print(llvm::raw_ostream &OS) const {
  OS << "StrongNode\n";
  for (const auto &Child : Children)
    Child.print(OS);
}

//===----------------------------------------------------------------------===//
// Block node print/dump
//===----------------------------------------------------------------------===//

LLVM_DUMP_METHOD void BlockNode::dump() const { print(llvm::errs()); }

void ParagraphNode::print(llvm::raw_ostream &OS) const {
  OS << "ParagraphNode\n";
  for (const auto &Child : Children)
    Child.print(OS);
}

void HeadingNode::print(llvm::raw_ostream &OS) const {
  OS << "HeadingNode: level=" << getLevel() << "\n";
  for (const auto &Child : Children)
    Child.print(OS);
}

void FencedCodeNode::print(llvm::raw_ostream &OS) const {
  OS << "FencedCodeNode: lang=" << getLang() << "\n" << getCode() << "\n";
}

void ListItemNode::print(llvm::raw_ostream &OS) const {
  OS << "ListItemNode\n";
  for (const auto &Child : Children)
    Child.print(OS);
}

LLVM_DUMP_METHOD void ListItemNode::dump() const { print(llvm::errs()); }

void UnorderedListNode::print(llvm::raw_ostream &OS) const {
  OS << "UnorderedListNode\n";
  for (const auto &Item : Items)
    Item.print(OS);
}

void OrderedListNode::print(llvm::raw_ostream &OS) const {
  OS << "OrderedListNode: start=" << getStart() << "\n";
  for (const auto &Item : Items)
    Item.print(OS);
}

void BlockQuoteNode::print(llvm::raw_ostream &OS) const {
  OS << "BlockQuoteNode\n";
  for (const auto &Child : Children)
    Child.print(OS);
}

void ThematicBreakNode::print(llvm::raw_ostream &OS) const {
  OS << "ThematicBreakNode\n";
}

void DocumentNode::print(llvm::raw_ostream &OS) const {
  OS << "DocumentNode\n";
  for (const auto &Child : Children)
    Child.print(OS);
}

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

static bool isListMarker(llvm::StringRef Line) {
  return Line.starts_with("- ") || Line.starts_with("* ") ||
         Line.starts_with("+ ");
}

static bool isThematicBreak(llvm::StringRef Line) {
  if (Line.empty())
    return false;
  char Marker = Line[0];
  if (Marker != '-' && Marker != '*' && Marker != '_')
    return false;
  llvm::SmallString<3> Allowed = {Marker, ' '};
  if (Line.find_first_not_of(llvm::StringRef(Allowed)) != llvm::StringRef::npos)
    return false;
  return Line.count(Marker) >= 3;
}

DocumentNode *parseMarkdown(llvm::StringRef Text, ASTContext &Ctx) {
  auto *Doc = Ctx.allocate<DocumentNode>();
  Ctx.setRoot(Doc);

  llvm::SmallVector<llvm::StringRef> Lines;
  Text.split(Lines, '\n');

  size_t I = 0;
  while (I < Lines.size()) {
    llvm::StringRef Line = Lines[I].trim();

    if (Line.empty()) {
      ++I;
      continue;
    }

    // Fenced code block
    if (Line.starts_with("```") || Line.starts_with("~~~")) {
      char Fence = Line[0];
      llvm::StringRef Lang = Line.drop_front(3).trim();
      ++I;
      llvm::SmallString<256> Code;
      while (I < Lines.size()) {
        llvm::StringRef Trimmed = Lines[I].trim();
        if (Trimmed.size() >= 3 && Trimmed[0] == Fence && Trimmed[1] == Fence &&
            Trimmed[2] == Fence) {
          ++I;
          break;
        }
        if (!Code.empty())
          Code += '\n';
        Code += Lines[I];
        ++I;
      }
      auto *Node = Ctx.allocate<FencedCodeNode>(Lang, Ctx.internString(Code));
      Doc->Children.push_back(*Node);
      continue;
    }

    // ATX heading: 1-6 # characters followed by a space
    if (Line.starts_with("#")) {
      unsigned Level = 0;
      while (Level < Line.size() && Line[Level] == '#')
        ++Level;
      if (Level <= 6 && Level < Line.size() && Line[Level] == ' ') {
        llvm::StringRef Content = Line.drop_front(Level + 1).trim();
        auto *Heading = Ctx.allocate<HeadingNode>(Level);
        auto *TNode = Ctx.allocate<TextNode>(Ctx.internString(Content));
        Heading->Children.push_back(*TNode);
        Doc->Children.push_back(*Heading);
        ++I;
        continue;
      }
    }

    // Thematic break: 3+ of -, *, or _ optionally separated by spaces
    if (isThematicBreak(Line)) {
      auto *Node = Ctx.allocate<ThematicBreakNode>();
      Doc->Children.push_back(*Node);
      ++I;
      continue;
    }

    // Unordered list
    if (isListMarker(Line)) {
      auto *List = Ctx.allocate<UnorderedListNode>();
      while (I < Lines.size()) {
        llvm::StringRef L = Lines[I].trim();
        if (!isListMarker(L))
          break;
        llvm::StringRef ItemText = L.drop_front(2).trim();
        auto *Item = Ctx.allocate<ListItemNode>();
        auto *TNode = Ctx.allocate<TextNode>(Ctx.internString(ItemText));
        Item->Children.push_back(*TNode);
        List->Items.push_back(*Item);
        ++I;
      }
      Doc->Children.push_back(*List);
      continue;
    }

    // Plain text paragraph
    llvm::SmallString<256> ParaText;
    while (I < Lines.size()) {
      llvm::StringRef L = Lines[I].trim();
      if (L.empty())
        break;
      if (L.starts_with("```") || L.starts_with("~~~"))
        break;
      if (isListMarker(L))
        break;
      if (L.starts_with("#"))
        break;
      if (isThematicBreak(L))
        break;
      if (!ParaText.empty())
        ParaText += ' ';
      ParaText += L;
      ++I;
    }
    if (!ParaText.empty()) {
      auto *Para = Ctx.allocate<ParagraphNode>();
      auto *TNode = Ctx.allocate<TextNode>(Ctx.internString(ParaText));
      Para->Children.push_back(*TNode);
      Doc->Children.push_back(*Para);
    }
  }

  return Doc;
}

} // namespace clang::doc::markdown
