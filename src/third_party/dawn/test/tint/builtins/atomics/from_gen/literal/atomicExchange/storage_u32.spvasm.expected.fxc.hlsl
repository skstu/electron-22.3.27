RWByteAddressBuffer sb_rw : register(u0, space0);

uint tint_atomicExchange(RWByteAddressBuffer buffer, uint offset, uint value) {
  uint original_value = 0;
  buffer.InterlockedExchange(offset, value, original_value);
  return original_value;
}


void atomicExchange_d59712() {
  uint res = 0u;
  const uint x_9 = tint_atomicExchange(sb_rw, 0u, 1u);
  res = x_9;
  return;
}

void fragment_main_1() {
  atomicExchange_d59712();
  return;
}

void fragment_main() {
  fragment_main_1();
  return;
}

void compute_main_1() {
  atomicExchange_d59712();
  return;
}

[numthreads(1, 1, 1)]
void compute_main() {
  compute_main_1();
  return;
}
