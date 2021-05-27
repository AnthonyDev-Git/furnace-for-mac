#include "gb.h"
#include "../engine.h"
#include <math.h>

//#define rWrite(a,v) pendingWrites[a]=v;
#define rWrite(a,v) GB_apu_write(gb,a,v);

#define FREQ_BASE 7943.75f

void DivPlatformGB::acquire(int& l, int& r) {
  gb->apu.apu_cycles=4;
  GB_apu_run(gb);
  l=(gb->apu_output.current_sample[0].left+
    gb->apu_output.current_sample[1].left+
    gb->apu_output.current_sample[2].left+
    gb->apu_output.current_sample[3].left)<<6;
  r=(gb->apu_output.current_sample[0].right+
    gb->apu_output.current_sample[1].right+
    gb->apu_output.current_sample[2].right+
    gb->apu_output.current_sample[3].right)<<6;
}

void DivPlatformGB::tick() {
  for (int i=0; i<4; i++) {
    chan[i].std.next();
    if (chan[i].std.hadArp) {
      if (chan[i].std.arpMode) {
        chan[i].baseFreq=round(FREQ_BASE/pow(2.0f,((float)(chan[i].std.arp)/12.0f)));
      } else {
        chan[i].baseFreq=round(FREQ_BASE/pow(2.0f,((float)(chan[i].note+chan[i].std.arp-12)/12.0f)));
      }
      chan[i].freqChanged=true;
    }
    if (chan[i].std.hadDuty) {
      chan[i].duty=chan[i].std.duty;
      rWrite(16+i*5+1,(chan[i].duty&3)<<6);
    }
    if (chan[i].freqChanged) {
      chan[i].freq=(chan[i].baseFreq*(ONE_SEMITONE-chan[i].pitch))/ONE_SEMITONE;
      if (chan[i].note>0x5d) chan[i].freq=0x01;
      if (i==0 || i==1) {
        if (chan[i].keyOn) {
          DivInstrument* ins=parent->getIns(chan[i].ins);
          rWrite(16+i*5+2,((chan[i].vol*ins->gb.envVol)&0xf0)|(ins->gb.envLen&7)|((ins->gb.envDir&1)<<3));
        }
        rWrite(16+i*5+3,(2048-chan[i].freq)&0xff);
        rWrite(16+i*5+4,(((2048-chan[i].freq)>>8)&7)|(chan[i].keyOn?0x80:0x00));
        if (chan[i].keyOn) chan[i].keyOn=false;
      }
      chan[i].freqChanged=false;
    }
  }

  for (int i=0; i<64; i++) {
    if (pendingWrites[i]!=oldWrites[i]) {
      GB_apu_write(gb,i,pendingWrites[i]&0xff);
      oldWrites[i]=pendingWrites[i];
    }
  }
}

int DivPlatformGB::dispatch(DivCommand c) {
  switch (c.cmd) {
    case DIV_CMD_NOTE_ON:
      chan[c.chan].baseFreq=round(FREQ_BASE/pow(2.0f,((float)c.value/12.0f)));
      chan[c.chan].freqChanged=true;
      chan[c.chan].note=c.value;
      chan[c.chan].active=true;
      chan[c.chan].keyOn=true;
      chan[c.chan].std.init(parent->getIns(chan[c.chan].ins));
      break;
    case DIV_CMD_NOTE_OFF:
      chan[c.chan].active=false;
      chan[c.chan].keyOff=true;
      chan[c.chan].std.init(NULL);
      break;
    case DIV_CMD_INSTRUMENT:
      chan[c.chan].ins=c.value;
      break;
    case DIV_CMD_VOLUME:
      if (chan[c.chan].vol!=c.value) {
        chan[c.chan].vol=c.value;
      }
      break;
    case DIV_CMD_GET_VOLUME:
      return chan[c.chan].vol;
      break;
    case DIV_CMD_PITCH:
      chan[c.chan].pitch=c.value;
      chan[c.chan].freqChanged=true;
      break;
    case DIV_CMD_NOTE_PORTA: {
      int destFreq=round(FREQ_BASE/pow(2.0f,((float)c.value2/12.0f)));
      bool return2=false;
      if (destFreq>chan[c.chan].baseFreq) {
        chan[c.chan].baseFreq+=c.value;
        if (chan[c.chan].baseFreq>=destFreq) {
          chan[c.chan].baseFreq=destFreq;
          return2=true;
        }
      } else {
        chan[c.chan].baseFreq-=c.value;
        if (chan[c.chan].baseFreq<=destFreq) {
          chan[c.chan].baseFreq=destFreq;
          return2=true;
        }
      }
      chan[c.chan].freqChanged=true;
      if (return2) return 2;
      break;
    }
    case DIV_CMD_STD_NOISE_MODE:
      snNoiseMode=(c.value&1)|((c.value&16)>>3);
      updateSNMode=true;
      break;
    case DIV_CMD_LEGATO:
      chan[c.chan].baseFreq=round(FREQ_BASE/pow(2.0f,((float)c.value/12.0f)));
      chan[c.chan].freqChanged=true;
      chan[c.chan].note=c.value;
      break;
    case DIV_CMD_PRE_PORTA:
      chan[c.chan].std.init(parent->getIns(chan[c.chan].ins));
      break;
    case DIV_CMD_GET_VOLMAX:
      return 15;
      break;
    default:
      break;
  }
  return 1;
}

int DivPlatformGB::init(DivEngine* p, int channels, int sugRate) {
  parent=p;
  rate=2097152;
  gb=new GB_gameboy_t;
  memset(gb,0,sizeof(GB_gameboy_t));
  gb->model=GB_MODEL_DMG_B;
  GB_apu_init(gb);
  GB_set_sample_rate(gb,rate);
  for (int i=0; i<64; i++) {
    oldWrites[i]=-1;
    pendingWrites[i]=-1;
  }
  // enable all channels
  GB_apu_write(gb,0x26,0x80);
  GB_apu_write(gb,0x25,0xff);
  snNoiseMode=3;
  updateSNMode=false;
  return 4;
}