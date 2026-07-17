/* AudioWorklet processor for Ymir Web.
 * Receives interleaved S16 stereo chunks from the main thread and feeds the
 * WebAudio graph with Float32 planar output. Ring buffer absorbs timing jitter
 * between the rAF-driven emulator and the audio hardware clock.
 */
class YmirAudioProcessor extends AudioWorkletProcessor {
    constructor() {
        super();
        this.capacity = 44100; // 1 second per channel
        this.left = new Float32Array(this.capacity);
        this.right = new Float32Array(this.capacity);
        this.readPos = 0;
        this.writePos = 0;
        this.count = 0;
        this.port.onmessage = (e) => {
            const s = e.data; // Int16Array interleaved [L, R, L, R, ...]
            for (let i = 0; i + 1 < s.length; i += 2) {
                if (this.count >= this.capacity) {
                    // Overflow: drop oldest frame to bound latency.
                    this.readPos = (this.readPos + 1) % this.capacity;
                    this.count--;
                }
                this.left[this.writePos] = s[i] / 32768;
                this.right[this.writePos] = s[i + 1] / 32768;
                this.writePos = (this.writePos + 1) % this.capacity;
                this.count++;
            }
        };
    }

    process(inputs, outputs) {
        const out = outputs[0];
        const L = out[0];
        const R = out.length > 1 ? out[1] : out[0];
        for (let i = 0; i < L.length; i++) {
            if (this.count > 0) {
                L[i] = this.left[this.readPos];
                R[i] = this.right[this.readPos];
                this.readPos = (this.readPos + 1) % this.capacity;
                this.count--;
            } else {
                L[i] = 0;
                R[i] = 0;
            }
        }
        return true;
    }
}

registerProcessor("ymir-audio", YmirAudioProcessor);
