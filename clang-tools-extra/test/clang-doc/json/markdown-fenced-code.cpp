// RUN: rm -rf %t && mkdir -p %t
// RUN: clang-doc --pretty-json --output=%t --format=html --executor=standalone %s
// RUN: FileCheck %s < %t/json/GlobalNamespace/index.json

/// A function with a fenced code example.
///
/// ```cpp
/// int x = 0;
/// ```
void myFunction();

// CHECK:       "Description": {
// CHECK-NEXT:    "HasParagraphComments": true,
// CHECK-NEXT:    "ParagraphComments": [
// CHECK-NEXT:      [
// CHECK-NEXT:        {
// CHECK-NEXT:          "TextComment": "A function with a fenced code example."
// CHECK-NEXT:        }
// CHECK-NEXT:      ],
// CHECK-NEXT:      [
// CHECK-NEXT:        {
// CHECK-NEXT:          "TextComment": "```cpp"
// CHECK-NEXT:        },
// CHECK-NEXT:        {
// CHECK-NEXT:          "TextComment": "int x = 0;"
// CHECK-NEXT:        },
// CHECK-NEXT:        {
// CHECK-NEXT:          "TextComment": "```"
// CHECK-NEXT:        }
// CHECK-NEXT:      ]
// CHECK-NEXT:    ],
// CHECK-NEXT:    "ParsedMarkdown": [
// CHECK-NEXT:      {
// CHECK-NEXT:        "Lang": "cpp",
// CHECK-NEXT:        "Lines": [
// CHECK-NEXT:          "int x = 0;"
// CHECK-NEXT:        ],
// CHECK-NEXT:        "Type": "FencedCode"
// CHECK-NEXT:      }
// CHECK-NEXT:    ]
// CHECK-NEXT:  },
