// RUN: rm -rf %t && mkdir -p %t
// RUN: clang-doc --pretty-json --output=%t --format=html --executor=standalone %s
// RUN: FileCheck %s < %t/json/GlobalNamespace/index.json

/// A function with a table.
///
/// | Name | Value |
/// | --- | --- |
/// | a | 1 |
/// | b | 2 |
void myFunction();

// CHECK:       "Description": {
// CHECK-NEXT:    "HasParagraphComments": true,
// CHECK-NEXT:    "ParagraphComments": [
// CHECK-NEXT:      [
// CHECK-NEXT:        {
// CHECK-NEXT:          "TextComment": "A function with a table."
// CHECK-NEXT:        }
// CHECK-NEXT:      ],
// CHECK-NEXT:      [
// CHECK-NEXT:        {
// CHECK-NEXT:          "TextComment": "| Name | Value |"
// CHECK-NEXT:        },
// CHECK-NEXT:        {
// CHECK-NEXT:          "TextComment": "| --- | --- |"
// CHECK-NEXT:        },
// CHECK-NEXT:        {
// CHECK-NEXT:          "TextComment": "| a | 1 |"
// CHECK-NEXT:        },
// CHECK-NEXT:        {
// CHECK-NEXT:          "TextComment": "| b | 2 |"
// CHECK-NEXT:        }
// CHECK-NEXT:      ]
// CHECK-NEXT:    ],
// CHECK-NEXT:    "ParsedMarkdown": [
// CHECK-NEXT:      {
// CHECK-NEXT:        "Body": [
// CHECK-NEXT:          [
// CHECK-NEXT:            "a",
// CHECK-NEXT:            "1"
// CHECK-NEXT:          ],
// CHECK-NEXT:          [
// CHECK-NEXT:            "b",
// CHECK-NEXT:            "2"
// CHECK-NEXT:          ]
// CHECK-NEXT:        ],
// CHECK-NEXT:        "Header": [
// CHECK-NEXT:          "Name",
// CHECK-NEXT:          "Value"
// CHECK-NEXT:        ],
// CHECK-NEXT:        "Type": "Table"
// CHECK-NEXT:      }
// CHECK-NEXT:    ]
// CHECK-NEXT:  },
