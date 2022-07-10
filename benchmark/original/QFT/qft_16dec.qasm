OPENQASM 2.0;
include "qelib1.inc";
qreg q[16];
x q[0];
rz(pi) q[0];
x q[0];
rz(-pi/4.0E+00) q[0];
cx q[1],q[0];
rz(pi/4.0E+00) q[1];
rz(pi/4.0E+00) q[0];
rz(-pi/8.0E+00) q[0];
cx q[2],q[0];
rz(pi/8.0E+00) q[2];
rz(pi/8.0E+00) q[0];
rz(-pi/1.6E+01) q[0];
cx q[3],q[0];
rz(pi/1.6E+01) q[3];
rz(pi/1.6E+01) q[0];
rz(-pi/3.2E+01) q[0];
cx q[4],q[0];
rz(pi/3.2E+01) q[4];
rz(pi/3.2E+01) q[0];
rz(-pi/6.4E+01) q[0];
cx q[5],q[0];
rz(pi/6.4E+01) q[5];
rz(pi/6.4E+01) q[0];
rz(-pi/1.3E+02) q[0];
cx q[6],q[0];
rz(pi/1.3E+02) q[6];
rz(pi/1.3E+02) q[0];
rz(-pi/2.6E+02) q[0];
cx q[7],q[0];
rz(pi/2.6E+02) q[7];
rz(pi/2.6E+02) q[0];
rz(-pi/5.1E+02) q[0];
cx q[8],q[0];
rz(pi/5.1E+02) q[8];
rz(pi/5.1E+02) q[0];
rz(-pi/1.0E+03) q[0];
cx q[9],q[0];
rz(pi/1.0E+03) q[9];
rz(pi/1.0E+03) q[0];
rz(-pi/2.0E+03) q[0];
cx q[10],q[0];
rz(pi/2.0E+03) q[10];
rz(pi/2.0E+03) q[0];
rz(-pi/4.1E+03) q[0];
cx q[11],q[0];
rz(pi/4.1E+03) q[11];
rz(pi/4.1E+03) q[0];
rz(-pi/8.2E+03) q[0];
cx q[12],q[0];
rz(pi/8.2E+03) q[12];
rz(pi/8.2E+03) q[0];
rz(-pi/1.6E+04) q[0];
cx q[13],q[0];
rz(pi/1.6E+04) q[13];
rz(pi/1.6E+04) q[0];
rz(-pi/3.3E+04) q[0];
cx q[14],q[0];
rz(pi/3.3E+04) q[14];
rz(pi/3.3E+04) q[0];
rz(-pi/6.6E+04) q[0];
cx q[15],q[0];
rz(pi/6.6E+04) q[15];
rz(pi/6.6E+04) q[0];
x q[1];
rz(pi) q[1];
x q[1];
rz(-pi/4.0E+00) q[1];
cx q[2],q[1];
rz(pi/4.0E+00) q[2];
rz(pi/4.0E+00) q[1];
rz(-pi/8.0E+00) q[1];
cx q[3],q[1];
rz(pi/8.0E+00) q[3];
rz(pi/8.0E+00) q[1];
rz(-pi/1.6E+01) q[1];
cx q[4],q[1];
rz(pi/1.6E+01) q[4];
rz(pi/1.6E+01) q[1];
rz(-pi/3.2E+01) q[1];
cx q[5],q[1];
rz(pi/3.2E+01) q[5];
rz(pi/3.2E+01) q[1];
rz(-pi/6.4E+01) q[1];
cx q[6],q[1];
rz(pi/6.4E+01) q[6];
rz(pi/6.4E+01) q[1];
rz(-pi/1.3E+02) q[1];
cx q[7],q[1];
rz(pi/1.3E+02) q[7];
rz(pi/1.3E+02) q[1];
rz(-pi/2.6E+02) q[1];
cx q[8],q[1];
rz(pi/2.6E+02) q[8];
rz(pi/2.6E+02) q[1];
rz(-pi/5.1E+02) q[1];
cx q[9],q[1];
rz(pi/5.1E+02) q[9];
rz(pi/5.1E+02) q[1];
rz(-pi/1.0E+03) q[1];
cx q[10],q[1];
rz(pi/1.0E+03) q[10];
rz(pi/1.0E+03) q[1];
rz(-pi/2.0E+03) q[1];
cx q[11],q[1];
rz(pi/2.0E+03) q[11];
rz(pi/2.0E+03) q[1];
rz(-pi/4.1E+03) q[1];
cx q[12],q[1];
rz(pi/4.1E+03) q[12];
rz(pi/4.1E+03) q[1];
rz(-pi/8.2E+03) q[1];
cx q[13],q[1];
rz(pi/8.2E+03) q[13];
rz(pi/8.2E+03) q[1];
rz(-pi/1.6E+04) q[1];
cx q[14],q[1];
rz(pi/1.6E+04) q[14];
rz(pi/1.6E+04) q[1];
rz(-pi/3.3E+04) q[1];
cx q[15],q[1];
rz(pi/3.3E+04) q[15];
rz(pi/3.3E+04) q[1];
x q[2];
rz(pi) q[2];
x q[2];
rz(-pi/4.0E+00) q[2];
cx q[3],q[2];
rz(pi/4.0E+00) q[3];
rz(pi/4.0E+00) q[2];
rz(-pi/8.0E+00) q[2];
cx q[4],q[2];
rz(pi/8.0E+00) q[4];
rz(pi/8.0E+00) q[2];
rz(-pi/1.6E+01) q[2];
cx q[5],q[2];
rz(pi/1.6E+01) q[5];
rz(pi/1.6E+01) q[2];
rz(-pi/3.2E+01) q[2];
cx q[6],q[2];
rz(pi/3.2E+01) q[6];
rz(pi/3.2E+01) q[2];
rz(-pi/6.4E+01) q[2];
cx q[7],q[2];
rz(pi/6.4E+01) q[7];
rz(pi/6.4E+01) q[2];
rz(-pi/1.3E+02) q[2];
cx q[8],q[2];
rz(pi/1.3E+02) q[8];
rz(pi/1.3E+02) q[2];
rz(-pi/2.6E+02) q[2];
cx q[9],q[2];
rz(pi/2.6E+02) q[9];
rz(pi/2.6E+02) q[2];
rz(-pi/5.1E+02) q[2];
cx q[10],q[2];
rz(pi/5.1E+02) q[10];
rz(pi/5.1E+02) q[2];
rz(-pi/1.0E+03) q[2];
cx q[11],q[2];
rz(pi/1.0E+03) q[11];
rz(pi/1.0E+03) q[2];
rz(-pi/2.0E+03) q[2];
cx q[12],q[2];
rz(pi/2.0E+03) q[12];
rz(pi/2.0E+03) q[2];
rz(-pi/4.1E+03) q[2];
cx q[13],q[2];
rz(pi/4.1E+03) q[13];
rz(pi/4.1E+03) q[2];
rz(-pi/8.2E+03) q[2];
cx q[14],q[2];
rz(pi/8.2E+03) q[14];
rz(pi/8.2E+03) q[2];
rz(-pi/1.6E+04) q[2];
cx q[15],q[2];
rz(pi/1.6E+04) q[15];
rz(pi/1.6E+04) q[2];
x q[3];
rz(pi) q[3];
x q[3];
rz(-pi/4.0E+00) q[3];
cx q[4],q[3];
rz(pi/4.0E+00) q[4];
rz(pi/4.0E+00) q[3];
rz(-pi/8.0E+00) q[3];
cx q[5],q[3];
rz(pi/8.0E+00) q[5];
rz(pi/8.0E+00) q[3];
rz(-pi/1.6E+01) q[3];
cx q[6],q[3];
rz(pi/1.6E+01) q[6];
rz(pi/1.6E+01) q[3];
rz(-pi/3.2E+01) q[3];
cx q[7],q[3];
rz(pi/3.2E+01) q[7];
rz(pi/3.2E+01) q[3];
rz(-pi/6.4E+01) q[3];
cx q[8],q[3];
rz(pi/6.4E+01) q[8];
rz(pi/6.4E+01) q[3];
rz(-pi/1.3E+02) q[3];
cx q[9],q[3];
rz(pi/1.3E+02) q[9];
rz(pi/1.3E+02) q[3];
rz(-pi/2.6E+02) q[3];
cx q[10],q[3];
rz(pi/2.6E+02) q[10];
rz(pi/2.6E+02) q[3];
rz(-pi/5.1E+02) q[3];
cx q[11],q[3];
rz(pi/5.1E+02) q[11];
rz(pi/5.1E+02) q[3];
rz(-pi/1.0E+03) q[3];
cx q[12],q[3];
rz(pi/1.0E+03) q[12];
rz(pi/1.0E+03) q[3];
rz(-pi/2.0E+03) q[3];
cx q[13],q[3];
rz(pi/2.0E+03) q[13];
rz(pi/2.0E+03) q[3];
rz(-pi/4.1E+03) q[3];
cx q[14],q[3];
rz(pi/4.1E+03) q[14];
rz(pi/4.1E+03) q[3];
rz(-pi/8.2E+03) q[3];
cx q[15],q[3];
rz(pi/8.2E+03) q[15];
rz(pi/8.2E+03) q[3];
x q[4];
rz(pi) q[4];
x q[4];
rz(-pi/4.0E+00) q[4];
cx q[5],q[4];
rz(pi/4.0E+00) q[5];
rz(pi/4.0E+00) q[4];
rz(-pi/8.0E+00) q[4];
cx q[6],q[4];
rz(pi/8.0E+00) q[6];
rz(pi/8.0E+00) q[4];
rz(-pi/1.6E+01) q[4];
cx q[7],q[4];
rz(pi/1.6E+01) q[7];
rz(pi/1.6E+01) q[4];
rz(-pi/3.2E+01) q[4];
cx q[8],q[4];
rz(pi/3.2E+01) q[8];
rz(pi/3.2E+01) q[4];
rz(-pi/6.4E+01) q[4];
cx q[9],q[4];
rz(pi/6.4E+01) q[9];
rz(pi/6.4E+01) q[4];
rz(-pi/1.3E+02) q[4];
cx q[10],q[4];
rz(pi/1.3E+02) q[10];
rz(pi/1.3E+02) q[4];
rz(-pi/2.6E+02) q[4];
cx q[11],q[4];
rz(pi/2.6E+02) q[11];
rz(pi/2.6E+02) q[4];
rz(-pi/5.1E+02) q[4];
cx q[12],q[4];
rz(pi/5.1E+02) q[12];
rz(pi/5.1E+02) q[4];
rz(-pi/1.0E+03) q[4];
cx q[13],q[4];
rz(pi/1.0E+03) q[13];
rz(pi/1.0E+03) q[4];
rz(-pi/2.0E+03) q[4];
cx q[14],q[4];
rz(pi/2.0E+03) q[14];
rz(pi/2.0E+03) q[4];
rz(-pi/4.1E+03) q[4];
cx q[15],q[4];
rz(pi/4.1E+03) q[15];
rz(pi/4.1E+03) q[4];
x q[5];
rz(pi) q[5];
x q[5];
rz(-pi/4.0E+00) q[5];
cx q[6],q[5];
rz(pi/4.0E+00) q[6];
rz(pi/4.0E+00) q[5];
rz(-pi/8.0E+00) q[5];
cx q[7],q[5];
rz(pi/8.0E+00) q[7];
rz(pi/8.0E+00) q[5];
rz(-pi/1.6E+01) q[5];
cx q[8],q[5];
rz(pi/1.6E+01) q[8];
rz(pi/1.6E+01) q[5];
rz(-pi/3.2E+01) q[5];
cx q[9],q[5];
rz(pi/3.2E+01) q[9];
rz(pi/3.2E+01) q[5];
rz(-pi/6.4E+01) q[5];
cx q[10],q[5];
rz(pi/6.4E+01) q[10];
rz(pi/6.4E+01) q[5];
rz(-pi/1.3E+02) q[5];
cx q[11],q[5];
rz(pi/1.3E+02) q[11];
rz(pi/1.3E+02) q[5];
rz(-pi/2.6E+02) q[5];
cx q[12],q[5];
rz(pi/2.6E+02) q[12];
rz(pi/2.6E+02) q[5];
rz(-pi/5.1E+02) q[5];
cx q[13],q[5];
rz(pi/5.1E+02) q[13];
rz(pi/5.1E+02) q[5];
rz(-pi/1.0E+03) q[5];
cx q[14],q[5];
rz(pi/1.0E+03) q[14];
rz(pi/1.0E+03) q[5];
rz(-pi/2.0E+03) q[5];
cx q[15],q[5];
rz(pi/2.0E+03) q[15];
rz(pi/2.0E+03) q[5];
x q[6];
rz(pi) q[6];
x q[6];
rz(-pi/4.0E+00) q[6];
cx q[7],q[6];
rz(pi/4.0E+00) q[7];
rz(pi/4.0E+00) q[6];
rz(-pi/8.0E+00) q[6];
cx q[8],q[6];
rz(pi/8.0E+00) q[8];
rz(pi/8.0E+00) q[6];
rz(-pi/1.6E+01) q[6];
cx q[9],q[6];
rz(pi/1.6E+01) q[9];
rz(pi/1.6E+01) q[6];
rz(-pi/3.2E+01) q[6];
cx q[10],q[6];
rz(pi/3.2E+01) q[10];
rz(pi/3.2E+01) q[6];
rz(-pi/6.4E+01) q[6];
cx q[11],q[6];
rz(pi/6.4E+01) q[11];
rz(pi/6.4E+01) q[6];
rz(-pi/1.3E+02) q[6];
cx q[12],q[6];
rz(pi/1.3E+02) q[12];
rz(pi/1.3E+02) q[6];
rz(-pi/2.6E+02) q[6];
cx q[13],q[6];
rz(pi/2.6E+02) q[13];
rz(pi/2.6E+02) q[6];
rz(-pi/5.1E+02) q[6];
cx q[14],q[6];
rz(pi/5.1E+02) q[14];
rz(pi/5.1E+02) q[6];
rz(-pi/1.0E+03) q[6];
cx q[15],q[6];
rz(pi/1.0E+03) q[15];
rz(pi/1.0E+03) q[6];
x q[7];
rz(pi) q[7];
x q[7];
rz(-pi/4.0E+00) q[7];
cx q[8],q[7];
rz(pi/4.0E+00) q[8];
rz(pi/4.0E+00) q[7];
rz(-pi/8.0E+00) q[7];
cx q[9],q[7];
rz(pi/8.0E+00) q[9];
rz(pi/8.0E+00) q[7];
rz(-pi/1.6E+01) q[7];
cx q[10],q[7];
rz(pi/1.6E+01) q[10];
rz(pi/1.6E+01) q[7];
rz(-pi/3.2E+01) q[7];
cx q[11],q[7];
rz(pi/3.2E+01) q[11];
rz(pi/3.2E+01) q[7];
rz(-pi/6.4E+01) q[7];
cx q[12],q[7];
rz(pi/6.4E+01) q[12];
rz(pi/6.4E+01) q[7];
rz(-pi/1.3E+02) q[7];
cx q[13],q[7];
rz(pi/1.3E+02) q[13];
rz(pi/1.3E+02) q[7];
rz(-pi/2.6E+02) q[7];
cx q[14],q[7];
rz(pi/2.6E+02) q[14];
rz(pi/2.6E+02) q[7];
rz(-pi/5.1E+02) q[7];
cx q[15],q[7];
rz(pi/5.1E+02) q[15];
rz(pi/5.1E+02) q[7];
x q[8];
rz(pi) q[8];
x q[8];
rz(-pi/4.0E+00) q[8];
cx q[9],q[8];
rz(pi/4.0E+00) q[9];
rz(pi/4.0E+00) q[8];
rz(-pi/8.0E+00) q[8];
cx q[10],q[8];
rz(pi/8.0E+00) q[10];
rz(pi/8.0E+00) q[8];
rz(-pi/1.6E+01) q[8];
cx q[11],q[8];
rz(pi/1.6E+01) q[11];
rz(pi/1.6E+01) q[8];
rz(-pi/3.2E+01) q[8];
cx q[12],q[8];
rz(pi/3.2E+01) q[12];
rz(pi/3.2E+01) q[8];
rz(-pi/6.4E+01) q[8];
cx q[13],q[8];
rz(pi/6.4E+01) q[13];
rz(pi/6.4E+01) q[8];
rz(-pi/1.3E+02) q[8];
cx q[14],q[8];
rz(pi/1.3E+02) q[14];
rz(pi/1.3E+02) q[8];
rz(-pi/2.6E+02) q[8];
cx q[15],q[8];
rz(pi/2.6E+02) q[15];
rz(pi/2.6E+02) q[8];
x q[9];
rz(pi) q[9];
x q[9];
rz(-pi/4.0E+00) q[9];
cx q[10],q[9];
rz(pi/4.0E+00) q[10];
rz(pi/4.0E+00) q[9];
rz(-pi/8.0E+00) q[9];
cx q[11],q[9];
rz(pi/8.0E+00) q[11];
rz(pi/8.0E+00) q[9];
rz(-pi/1.6E+01) q[9];
cx q[12],q[9];
rz(pi/1.6E+01) q[12];
rz(pi/1.6E+01) q[9];
rz(-pi/3.2E+01) q[9];
cx q[13],q[9];
rz(pi/3.2E+01) q[13];
rz(pi/3.2E+01) q[9];
rz(-pi/6.4E+01) q[9];
cx q[14],q[9];
rz(pi/6.4E+01) q[14];
rz(pi/6.4E+01) q[9];
rz(-pi/1.3E+02) q[9];
cx q[15],q[9];
rz(pi/1.3E+02) q[15];
rz(pi/1.3E+02) q[9];
x q[10];
rz(pi) q[10];
x q[10];
rz(-pi/4.0E+00) q[10];
cx q[11],q[10];
rz(pi/4.0E+00) q[11];
rz(pi/4.0E+00) q[10];
rz(-pi/8.0E+00) q[10];
cx q[12],q[10];
rz(pi/8.0E+00) q[12];
rz(pi/8.0E+00) q[10];
rz(-pi/1.6E+01) q[10];
cx q[13],q[10];
rz(pi/1.6E+01) q[13];
rz(pi/1.6E+01) q[10];
rz(-pi/3.2E+01) q[10];
cx q[14],q[10];
rz(pi/3.2E+01) q[14];
rz(pi/3.2E+01) q[10];
rz(-pi/6.4E+01) q[10];
cx q[15],q[10];
rz(pi/6.4E+01) q[15];
rz(pi/6.4E+01) q[10];
x q[11];
rz(pi) q[11];
x q[11];
rz(-pi/4.0E+00) q[11];
cx q[12],q[11];
rz(pi/4.0E+00) q[12];
rz(pi/4.0E+00) q[11];
rz(-pi/8.0E+00) q[11];
cx q[13],q[11];
rz(pi/8.0E+00) q[13];
rz(pi/8.0E+00) q[11];
rz(-pi/1.6E+01) q[11];
cx q[14],q[11];
rz(pi/1.6E+01) q[14];
rz(pi/1.6E+01) q[11];
rz(-pi/3.2E+01) q[11];
cx q[15],q[11];
rz(pi/3.2E+01) q[15];
rz(pi/3.2E+01) q[11];
x q[12];
rz(pi) q[12];
x q[12];
rz(-pi/4.0E+00) q[12];
cx q[13],q[12];
rz(pi/4.0E+00) q[13];
rz(pi/4.0E+00) q[12];
rz(-pi/8.0E+00) q[12];
cx q[14],q[12];
rz(pi/8.0E+00) q[14];
rz(pi/8.0E+00) q[12];
rz(-pi/1.6E+01) q[12];
cx q[15],q[12];
rz(pi/1.6E+01) q[15];
rz(pi/1.6E+01) q[12];
x q[13];
rz(pi) q[13];
x q[13];
rz(-pi/4.0E+00) q[13];
cx q[14],q[13];
rz(pi/4.0E+00) q[14];
rz(pi/4.0E+00) q[13];
rz(-pi/8.0E+00) q[13];
cx q[15],q[13];
rz(pi/8.0E+00) q[15];
rz(pi/8.0E+00) q[13];
x q[14];
rz(pi) q[14];
x q[14];
rz(-pi/4.0E+00) q[14];
cx q[15],q[14];
rz(pi/4.0E+00) q[15];
rz(pi/4.0E+00) q[14];
x q[15];
rz(pi) q[15];
x q[15];