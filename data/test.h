//
// Created by Admin on 12/03/2025.
//

#pragma once

struct A {
  float a;
};
struct B : A {
  float b;
};
struct C : A {
  float c;
};
struct D : B, C {
  float d;
};
