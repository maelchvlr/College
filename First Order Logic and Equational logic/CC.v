Inductive bin : Set :=
| C0 : bin
| C1 : bin
| C0S : bin -> bin
| C1S : bin -> bin.

Inductive is_even : bin -> Prop :=
| is_even_O : is_even C0
| is_even_n : forall n : bin, is_even C0 -> is_even (C0S n).

Goal is_even (C0S (C1S C1)).
Proof.
  apply is_even_n.
  apply is_even_O.
Qed.

Fixpoint head_0 (b : bin) : bin :=
match b with 
| C0 => C0S C0
| C1 => C1S C0
| C0S n => (C0S (head_0 n))
| C1S n => (C1S (head_0 n))
end.

Eval compute in head_0 (C0S (C1S C1)). 

Inductive is_equal : bin -> bin -> Prop :=
| is_equal_refl : forall b: bin, (is_equal b b)
| is_equal_sym : forall b1 b2:bin, (is_equal b1 b2) -> (is_equal b2 b1)
| is_equal_trans : forall b1 b2 b3:bin, (is_equal b1 b2) /\ (is_equal b2 b3) -> (is_equal b1 b3)
| is_equal_0 : forall b1 b2: bin, (is_equal b1 b2) -> (is_equal b1 (head_0 b2)).

Goal is_equal (C0S (C1S C1)) (C0S (C1S (C1S (C0S (C0))))).
Proof.
  apply is_equal_0 with (b1:=(C0S (C1S C1))) (b2:=(C0S (C1S (C1S (C0))))).
  apply is_equal_0 with (b1:=(C0S (C1S C1))) (b2:=(C0S (C1S (C1)))).
  apply is_equal_refl.
Qed.

Definition mult_2 (b : bin) :=
C0S b.


Eval compute in mult_2 (C0S (C1S (C1))).

Goal forall b:bin, (is_even b) -> (exists b0 : bin, is_equal (b) (mult_2 b0)).
Proof.
  intros.
  induction b.
    - compute. exists C0. apply is_equal_0 with (b1:=(C0)) (b2:=(C0)). apply is_equal_refl.
    - compute. inversion H. 
    - compute. exists b. apply is_equal_refl.
    - compute. inversion H.
Qed.     






