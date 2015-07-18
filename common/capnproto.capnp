@0x991cf6cf4239c7c2;

struct WonkoFile {
  uid @0 :Text;
  name @1 :Text;
  versions @2 :List(WonkoVersion);

  struct WonkoVersion {
    version @0 :Text;
    type @1 :Text;
    time @2 :UInt32;
  }
}
