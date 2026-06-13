// RUN: rm -rf %t && mkdir -p %t
// RUN: clang-doc --pretty-json --output=%t --format=html --executor=standalone %s
// RUN: FileCheck %s < %t/json/GlobalNamespace/index.json

/// # Heading
///
/// 1. one
/// 2. two
///
/// ---
///
/// > quoted
void myFunction();

// CHECK:       "Description": {
// CHECK-NEXT:    "HasParagraphComments": true,
// CHECK-NEXT:    "ParagraphComments": [
// CHECK-NEXT:      [
// CHECK-NEXT:        {
// CHECK-NEXT:          "TextComment": "# Heading"
// CHECK-NEXT:        }
// CHECK-NEXT:      ],
// CHECK-NEXT:      [
// CHECK-NEXT:        {
// CHECK-NEXT:          "TextComment": "1. one"
// CHECK-NEXT:        },
// CHECK-NEXT:        {
// CHECK-NEXT:          "TextComment": "2. two"
// CHECK-NEXT:        }
// CHECK-NEXT:      ],
// CHECK-NEXT:      [
// CHECK-NEXT:        {
// CHECK-NEXT:          "TextComment": "---"
// CHECK-NEXT:        }
// CHECK-NEXT:      ],
// CHECK-NEXT:      [
// CHECK-NEXT:        {
// CHECK-NEXT:          "TextComment": "> quoted"
// CHECK-NEXT:        }
// CHECK-NEXT:      ]
// CHECK-NEXT:    ],
// CHECK-NEXT:    "ParsedMarkdown": [
// CHECK-NEXT:      {
// CHECK-NEXT:        "Level": 1,
// CHECK-NEXT:        "Text": "Heading",
// CHECK-NEXT:        "Type": "Heading"
// CHECK-NEXT:      },
// CHECK-NEXT:      {
// CHECK-NEXT:        "Items": [
// CHECK-NEXT:          "one",
// CHECK-NEXT:          "two"
// CHECK-NEXT:        ],
// CHECK-NEXT:        "Start": 1,
// CHECK-NEXT:        "Type": "OrderedList"
// CHECK-NEXT:      },
// CHECK-NEXT:      {
// CHECK-NEXT:        "Type": "ThematicBreak"
// CHECK-NEXT:      },
// CHECK-NEXT:      {
// CHECK-NEXT:        "Children": [
// CHECK-NEXT:          {
// CHECK-NEXT:            "Text": "quoted",
// CHECK-NEXT:            "Type": "Paragraph"
// CHECK-NEXT:          }
// CHECK-NEXT:        ],
// CHECK-NEXT:        "Type": "BlockQuote"
// CHECK-NEXT:      }
// CHECK-NEXT:    ]
// CHECK-NEXT:  },
