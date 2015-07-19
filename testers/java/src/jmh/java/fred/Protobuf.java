package fred;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

import com.google.protobuf.InvalidProtocolBufferException;
import org.openjdk.jmh.annotations.Benchmark;
import org.openjdk.jmh.annotations.Scope;
import org.openjdk.jmh.annotations.Setup;
import org.openjdk.jmh.annotations.State;
import org.openjdk.jmh.infra.Blackhole;
import org.openjdk.jmh.results.BenchmarkTaskResult;

@State(Scope.Thread)
public class Protobuf extends Common {
    private byte[] data;

    @Setup
    public void setup() throws IOException {
        this.data = Files.readAllBytes(Paths.get("protobuf.protobuf"));
    }

    @Benchmark
    public void parse() {
        try {
            WonkoFileProtos.WonkoFile.parseFrom(this.data);
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
    }
};
