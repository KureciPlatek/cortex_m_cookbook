% find_per_entropy.m
% Load an SRAM dump (format:  ADDR;VAL  hex, no 0x prefix), compute a
% sliding-window Shannon entropy over the byte values, and plot it.
% A key shows up as a high-entropy spike (near log2(window) bits).

clear all;
close all;

DUMP_FILE     = "sram_dump.csv";
SUBARRAY_SIZE = 64;                 % window size (bytes)

% ---- load ----------------------------------------------------------------
fid = fopen(DUMP_FILE, "r");
if fid < 0, error("cannot open %s", DUMP_FILE); end
C = textscan(fid, "%s %s", "Delimiter", ";");   % two hex columns
fclose(fid);

addr = hex2dec(C{1});               % decimal addresses (view: dec2hex(addr))
val  = hex2dec(C{2});               % byte values 0..255
Nv   = numel(val);

% ---- sliding-window entropy ---------------------------------------------
nwin = Nv - SUBARRAY_SIZE + 1;
H    = zeros(nwin, 1);

for i = 1:nwin
  w    = val(i:i + SUBARRAY_SIZE - 1);   % window
  c    = histc(w, 0:255);                % count of each possible byte value
  p    = c(c > 0) / SUBARRAY_SIZE;       % probabilities, drop empty bins
  H(i) = -sum(p .* log2(p));             % Shannon entropy (bits)
end

% ---- report + plot -------------------------------------------------------
[Hmax, imax] = max(H);
printf("max entropy %.3f bits at index %d (addr 0x%s)\n", ...
       Hmax, imax, dec2hex(addr(imax)));

plot(addr(1:nwin), H);
xlabel("address"); ylabel("H (bits)");
title(sprintf("Sliding Shannon entropy, window = %d bytes", SUBARRAY_SIZE));
grid on;
