using System;
using nested_structsNamespace;

public class runme {
  static void Main() {
    Outer outer = new Outer();
    nested_structs.setValues(outer, 10);

    Outer_inner1 inner1 = outer.getInner1();
    Outer_inner1 inner2 = outer.getInner2();
    Outer_inner1 inner3 = outer.getInner3();
    Outer_inner1 inner4 = outer.getInner4();
    if (inner1.getVal() != 10) throw new RuntimeException("failed inner1");
    if (inner2.getVal() != 20) throw new RuntimeException("failed inner2");
    if (inner3.getVal() != 20) throw new RuntimeException("failed inner3");
    if (inner4.getVal() != 40) throw new RuntimeException("failed inner4");

    Named inside1 = outer.getInside1();
    Named inside2 = outer.getInside2();
    Named inside3 = outer.getInside3();
    Named inside4 = outer.getInside4();
    if (inside1.getVal() != 100) throw new RuntimeException("failed inside1");
    if (inside2.getVal() != 200) throw new RuntimeException("failed inside2");
    if (inside3.getVal() != 200) throw new RuntimeException("failed inside3");
    if (inside4.getVal() != 400) throw new RuntimeException("failed inside4");
  }
}
