// vmpopc rd, vs2, vm
require(P.VU.vsew >= e8 && P.VU.vsew <= e64);
#if defined(DIFFTEST) && defined(CPU_XIANGSHAN) && defined(CONFIG_NO_DIRTY_VS)
require_vector_nodirty(true)
#else
require_vector(true);
#endif
reg_t vl = P.VU.vl->read();
reg_t rs2_num = insn.rs2();
require(P.VU.vstart->read() == 0);
reg_t popcount = 0;
for (reg_t i=P.VU.vstart->read(); i<vl; ++i) {
  const int midx = i / 32;
  const int mpos = i % 32;

  bool vs2_lsb = ((P.VU.elt<uint32_t>(rs2_num, midx ) >> mpos) & 0x1) == 1;
  if (insn.v_vm() == 1) {
    popcount += vs2_lsb;
  } else {
    bool do_mask = (P.VU.elt<uint32_t>(0, midx) >> mpos) & 0x1;
    popcount += (vs2_lsb && do_mask);
  }
}
WRITE_RD(popcount);
