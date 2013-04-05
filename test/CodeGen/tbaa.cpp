// RUN: %clang_cc1 -triple x86_64-apple-darwin -O1 -disable-llvm-optzns %s -emit-llvm -o - | FileCheck %s
// RUN: %clang_cc1 -triple x86_64-apple-darwin -O1 -struct-path-tbaa -disable-llvm-optzns %s -emit-llvm -o - | FileCheck %s -check-prefix=PATH
// Test TBAA metadata generated by front-end.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef struct
{
   uint16_t f16;
   uint32_t f32;
   uint16_t f16_2;
   uint32_t f32_2;
} StructA;
typedef struct
{
   uint16_t f16;
   StructA a;
   uint32_t f32;
} StructB;
typedef struct
{
   uint16_t f16;
   StructB b;
   uint32_t f32;
} StructC;
typedef struct
{
   uint16_t f16;
   StructB b;
   uint32_t f32;
   uint8_t f8;
} StructD;

typedef struct
{
   uint16_t f16;
   uint32_t f32;
} StructS;
typedef struct
{
   uint16_t f16;
   uint32_t f32;
} StructS2;

uint32_t g(uint32_t *s, StructA *A, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa !4
// CHECK: store i32 4, i32* %{{.*}}, align 4, !tbaa !4
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa !4
// PATH: store i32 4, i32* %{{.*}}, align 4, !tbaa !5
  *s = 1;
  A->f32 = 4;
  return *s;
}

uint32_t g2(uint32_t *s, StructA *A, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa !4
// CHECK: store i16 4, i16* %{{.*}}, align 2, !tbaa !5
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa !4
// PATH: store i16 4, i16* %{{.*}}, align 2, !tbaa !8
  *s = 1;
  A->f16 = 4;
  return *s;
}

uint32_t g3(StructA *A, StructB *B, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa !4
// CHECK: store i32 4, i32* %{{.*}}, align 4, !tbaa !4
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa !5
// PATH: store i32 4, i32* %{{.*}}, align 4, !tbaa !9
  A->f32 = 1;
  B->a.f32 = 4;
  return A->f32;
}

uint32_t g4(StructA *A, StructB *B, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa !4
// CHECK: store i16 4, i16* %{{.*}}, align 2, !tbaa !5
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa !5
// PATH: store i16 4, i16* %{{.*}}, align 2, !tbaa !11
  A->f32 = 1;
  B->a.f16 = 4;
  return A->f32;
}

uint32_t g5(StructA *A, StructB *B, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa !4
// CHECK: store i32 4, i32* %{{.*}}, align 4, !tbaa !4
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa !5
// PATH: store i32 4, i32* %{{.*}}, align 4, !tbaa !12
  A->f32 = 1;
  B->f32 = 4;
  return A->f32;
}

uint32_t g6(StructA *A, StructB *B, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa !4
// CHECK: store i32 4, i32* %{{.*}}, align 4, !tbaa !4
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa !5
// PATH: store i32 4, i32* %{{.*}}, align 4, !tbaa !13
  A->f32 = 1;
  B->a.f32_2 = 4;
  return A->f32;
}

uint32_t g7(StructA *A, StructS *S, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa !4
// CHECK: store i32 4, i32* %{{.*}}, align 4, !tbaa !4
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa !5
// PATH: store i32 4, i32* %{{.*}}, align 4, !tbaa !14
  A->f32 = 1;
  S->f32 = 4;
  return A->f32;
}

uint32_t g8(StructA *A, StructS *S, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa !4
// CHECK: store i16 4, i16* %{{.*}}, align 2, !tbaa !5
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa !5
// PATH: store i16 4, i16* %{{.*}}, align 2, !tbaa !16
  A->f32 = 1;
  S->f16 = 4;
  return A->f32;
}

uint32_t g9(StructS *S, StructS2 *S2, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa !4
// CHECK: store i32 4, i32* %{{.*}}, align 4, !tbaa !4
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa !14
// PATH: store i32 4, i32* %{{.*}}, align 4, !tbaa !17
  S->f32 = 1;
  S2->f32 = 4;
  return S->f32;
}

uint32_t g10(StructS *S, StructS2 *S2, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa !4
// CHECK: store i16 4, i16* %{{.*}}, align 2, !tbaa !5
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa !14
// PATH: store i16 4, i16* %{{.*}}, align 2, !tbaa !19
  S->f32 = 1;
  S2->f16 = 4;
  return S->f32;
}

uint32_t g11(StructC *C, StructD *D, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa !4
// CHECK: store i32 4, i32* %{{.*}}, align 4, !tbaa !4
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa !20
// PATH: store i32 4, i32* %{{.*}}, align 4, !tbaa !22
  C->b.a.f32 = 1;
  D->b.a.f32 = 4;
  return C->b.a.f32;
}

uint32_t g12(StructC *C, StructD *D, uint64_t count) {
// CHECK: define i32 @{{.*}}(
// CHECK: store i32 1, i32* %{{.*}}, align 4, !tbaa !4
// CHECK: store i32 4, i32* %{{.*}}, align 4, !tbaa !4
// TODO: differentiate the two accesses.
// PATH: define i32 @{{.*}}(
// PATH: store i32 1, i32* %{{.*}}, align 4, !tbaa !9
// PATH: store i32 4, i32* %{{.*}}, align 4, !tbaa !9
  StructB *b1 = &(C->b);
  StructB *b2 = &(D->b);
  // b1, b2 have different context.
  b1->a.f32 = 1;
  b2->a.f32 = 4;
  return b1->a.f32;
}

// CHECK: !1 = metadata !{metadata !"omnipotent char", metadata !2}
// CHECK: !2 = metadata !{metadata !"Simple C/C++ TBAA"}
// CHECK: !4 = metadata !{metadata !"int", metadata !1}
// CHECK: !5 = metadata !{metadata !"short", metadata !1}

// PATH: !1 = metadata !{metadata !"omnipotent char", metadata !2}
// PATH: !4 = metadata !{metadata !"int", metadata !1}
// PATH: !5 = metadata !{metadata !6, metadata !4, i64 4}
// PATH: !6 = metadata !{metadata !"_ZTS7StructA", i64 0, metadata !7, i64 4, metadata !4}
// PATH: !7 = metadata !{metadata !"short", metadata !1}
// PATH: !8 = metadata !{metadata !6, metadata !7, i64 0}
// PATH: !9 = metadata !{metadata !10, metadata !4, i64 8}
// PATH: !10 = metadata !{metadata !"_ZTS7StructB", i64 0, metadata !7, i64 4, metadata !6, i64 20, metadata !4}
// PATH: !11 = metadata !{metadata !10, metadata !7, i64 4}
// PATH: !12 = metadata !{metadata !10, metadata !4, i64 20}
// PATH: !13 = metadata !{metadata !10, metadata !4, i64 16}
// PATH: !14 = metadata !{metadata !15, metadata !4, i64 4}
// PATH: !15 = metadata !{metadata !"_ZTS7StructS", i64 0, metadata !7, i64 4, metadata !4}
// PATH: !16 = metadata !{metadata !15, metadata !7, i64 0}
// PATH: !17 = metadata !{metadata !18, metadata !4, i64 4}
// PATH: !18 = metadata !{metadata !"_ZTS8StructS2", i64 0, metadata !7, i64 4, metadata !4}
// PATH: !19 = metadata !{metadata !18, metadata !7, i64 0}
// PATH: !20 = metadata !{metadata !21, metadata !4, i64 12}
// PATH: !21 = metadata !{metadata !"_ZTS7StructC", i64 0, metadata !7, i64 4, metadata !10, i64 28, metadata !4}
// PATH: !22 = metadata !{metadata !23, metadata !4, i64 12}
// PATH: !23 = metadata !{metadata !"_ZTS7StructD", i64 0, metadata !7, i64 4, metadata !10, i64 28, metadata !4, i64 32, metadata !1}