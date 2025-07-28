#ifndef CHESS_ENGINE_H
#define CHESS_ENGINE_H

#include <Arduino.h>
#include <stdint.h>

#define W while
#define M 0x88
#define S 128
#define I 8000

#define MYRAND_MAX 65535     /* 16bit pseudo random generator */

class ChessEngine {
private:
  long N = 0, T = 0;                  /* N=evaluated positions+S, T=recursion limit */
  short Q = 0, O = 0, K = 0, R = 0, k = 16;    /* k=moving side */
  char *p = nullptr, c[5] = {0}, Z = 0;            /* p=pointer to c, c=user input/computer output, Z=recursion counter */
  signed char L = 0,
    w[8] = {0,2,2,7,-1,8,12,23},                             /* relative piece values */
    o[32] = { /* step-vector lists & init setup */
      -16,-15,-17,0,1,16,0,1,16,15,17,0,14,18,31,33,0,
      7,-1,11,6,8,3,6,
      6,3,5,7,4,5,3,6
    };
  char b[16*8+1] = { /* board array */
    22,19,21,23,20,21,19,22,28,21,16,13,12,13,16,21,
    18,18,18,18,18,18,18,18,22,15,10,7,6,7,10,15,
     0, 0, 0, 0, 0, 0, 0, 0,18,11,6,3,2,3,6,11,
     0, 0, 0, 0, 0, 0, 0, 0,16, 9,4,1,0,1,4,9,
     0, 0, 0, 0, 0, 0, 0, 0,16, 9,4,1,0,1,4,9,
     0, 0, 0, 0, 0, 0, 0, 0,18,11,6,3,2,3,6,11,
     9, 9, 9, 9, 9, 9, 9, 9,22,15,10,7,6,7,10,15,
    14,11,13,15,12,13,11,14,28,21,16,13,12,13,16,21,0
  };
  char bk[16*8+1] = {0};
  uint32_t seed = 0;                 /* 32-bit RNG state */
  char sym[17] = ".?pnkbrq?P?NKBRQ";
  int mn = 1;
  char lastM[5] = {0};

  // history of the last 6 computer moves
  char hist[6][5] = {{0}};
  int histCount = 0;

public:
  // Seed the internal RNG
  void setSeed(unsigned int s);

  // Core engine functions
  short D(short q, short l, short e, unsigned char E, unsigned char z, unsigned char n);
  void getserialchar();
  unsigned short myrand(void);
  void gameOver();
  void bkp();
  void serialBoard();
  bool playComputerMove(int depth); //returns false if gameOver
  String printMoveAndBoard();
  void waitForEnter();
  int readNumber();
  String getLastMove();
  void pickRandomMove();
};

// Implementations

inline void ChessEngine::setSeed(unsigned int s) {
  seed = s;
}

inline unsigned short ChessEngine::myrand(void) {
  seed = seed * 1103515245 + 12345;
  return (unsigned short)(seed >> 16);
}

inline void ChessEngine::getserialchar() {
  while (Serial.available()) {
    Serial.read();
  }
}

inline void ChessEngine::pickRandomMove() {
  const int dirs[8] = {17,15,16,1,-1,-16,-17,-15};
  unsigned char x, y;
  while (true) {
    x = myrand() & 0x3F;
    if (!(b[x] & k)) continue;
    int di = myrand() % 8;
    y = x + dirs[di];
    if (y & M) continue;
    if (b[y] & k) continue;
    c[0] = 'a' + (x & 7);
    c[1] = '8' - (x >> 4);
    c[2] = 'a' + (y & 7);
    c[3] = '8' - (y >> 4);
    c[4] = 0;
    break;
  }
}

inline short ChessEngine::D(short q, short l, short e, unsigned char E, unsigned char z, unsigned char n) {
  short m, v, i, P, V, s;
  unsigned char t, p, u, x, y, X, Y, H, B, j, d, h, F, G, C;
  signed char r;
  if (++Z > 30) {
    --Z; return e;
  }
  q--;
  k ^= 24;
  d = Y = 0;
  X = myrand() & ~M;
  W(d++ < n || d < 3 ||
    z & K == I && (N < T & d < 98 ||
                   (K = X, L = Y & ~M, d = 3))) {
    x = B = X;
    h = Y & S;
    P = d < 3 ? I : D(-l, 1 - l, -e, S, 0, d - 3);
    m = -P < l | R > 35 ? d > 2 ? -I : e : -P;
    ++N;
    do {
      u = b[x];
      if (u & k) {
        r = p = u & 7;
        j = o[p + 16];
        W(r = p > 2 & r < 0 ? -r : -o[++j]) {
          A:
          y = x; F = G = S;
          do {
            H = y = h ? Y ^ h : y + r;
            if (y & M) break;
            m = E - S & b[E] && y - E < 2 & E - y < 2 ? I : m;
            if (p < 3 & y == E) H ^= 16;
            t = b[H]; if (t & k | p < 3 & !(y - x & 7) - !t) break;
            i = 37 * w[t & 7] + (t & 192);
            m = i < 0 ? I : m;
            if (m >= l & d > 1) goto C;
            v = d - 1 ? e : i - p;
            if (d - !t > 1) {
              v = p < 6 ? b[x + 8] - b[y + 8] : 0;
              b[G] = b[H] = b[x] = 0; b[y] = u | 32;
              if (!(G & M)) b[F] = k + 6, v += 50;
              v -= p - 4 | R > 29 ? 0 : 20;
              if (p < 3) {
                v -= 9 * ((x - 2 & M || b[x - 2] - u) +
                          (x + 2 & M || b[x + 2] - u) - 1
                          + (b[x ^ 16] == k + 36))
                     - (R >> 2);
                V = y + r + 1 & S ? 647 - p : 2 * (u & y + 16 & 32);
                b[y] += V; i += V;
              }
              v += e + i; V = m > q ? m : q;
              C = d - 1 - (d > 5 & p > 2 & !t & !h);
              C = R > 29 | d < 3 | P - I ? C : d;
              do
                s = C > 2 | v > V ? -D(-l, -V, -v, F, 0, C) : v;
              W(s > q & ++C < d); v = s;
              if (z && K - I && v + I && x == K & y == L) {
                Q = -e - i; O = F;
                R += i >> 7; --Z; return l;
              }
              b[G] = k + 6; b[F] = b[y] = 0; b[x] = u; b[H] = t;
            }
                {
              // small bias in [-3..+3] to break ties at every node
              short rnd = (short)(myrand() & 0x7) - 3;
              if (v + rnd > m) {
                m = v;
                X = x;
                Y = y | S & F;
              }
            }
            if (h) { h = 0; goto A; }
            if (x + r - y | u & 32 | p > 2 & (p - 4 | j - 7 || b[G = x + 3 ^ r >> 1 & 7] - k - 6 || b[G ^ 1] | b[G ^ 2]))
               t += p < 5;
            else F = y;
          } W(!t);
        }
      }
    } W((x = x + 9 & ~M) - B);
    C:
    if (m > I - M | m < M - I) d = 98;
    m = m + I | P == I ? m : 0;
    if (z && d > 2) {
      *c = 'a' + (X & 7);
      c[1] = '8' - (X >> 4);
      c[2] = 'a' + (Y & 7);
      c[3] = '8' - (Y >> 4 & 7);
      c[4] = 0;
    }
  }
  k ^= 24;
  --Z;
  return m += m < e;
}

inline bool ChessEngine::playComputerMove(int depth) {
  // 1) Detect 3‐fold repetition FOR ONE SIDE ONLY
  bool breakLoop = false;
  if (histCount >= 6) {
    // even indices = one side, odd = the other
    bool evensRepeat = memcmp(hist[0], hist[2], 5) == 0
                    && memcmp(hist[2], hist[4],     5) == 0;
    bool oddsRepeat  = memcmp(hist[1], hist[3], 5) == 0
                    && memcmp(hist[3], hist[5],     5) == 0;
    if (evensRepeat || oddsRepeat) breakLoop = true;
  }

  // 2) If triggered, do a random legal move instead of search
  if (breakLoop) {
    pickRandomMove();
    histCount = 0;            // clear history
  }
  else {
    // normal alpha‐beta search
    K = I; N = 0; T = 0x3F;
    int score = D(-I, I, Q, O, 1, depth);
    if (!(score > -I + 1)) {
      gameOver();
      return(false);
    }
  }

  // 3) Shift history left & append this move
  for (int i = 0; i < 5; ++i) memcpy(hist[i], hist[i+1], 5);
  memcpy(hist[5], c, 5);
  if (histCount < 6) ++histCount;

  strcpy(lastM, c);
  ++mn;
  return(true);
}


inline String ChessEngine::printMoveAndBoard() {
  Serial.print(mn);
  Serial.print(k == 0x08 ? ". " : "... ");
  Serial.println(c);
  serialBoard();
  return(String(c));
}

inline void ChessEngine::serialBoard() {
  Serial.println("  +-----------------+");
  for (int i = 0; i < 8; i++) {
    Serial.print(' '); Serial.print(8-i); Serial.print("| ");
    for (int j = 0; j < 8; j++) {
      char sq = sym[b[16*i+j] & 15];
      Serial.print(sq); Serial.print(' ');
    }
    Serial.println('|');
  }
  Serial.println("  +-----------------+");
  Serial.println("   a b c d e f g h");
}

inline void ChessEngine::waitForEnter() {
  while (Serial.read() != '\n');
}

inline int ChessEngine::readNumber() {
  String s = Serial.readStringUntil('\n');
  s.trim(); int v = s.toInt(); if (v<=0) v=3; return v;
}

inline String ChessEngine::getLastMove() { return String(c); }

inline void ChessEngine::gameOver() {
  Serial.println("Game Over");
}

#endif // CHESS_ENGINE_H
