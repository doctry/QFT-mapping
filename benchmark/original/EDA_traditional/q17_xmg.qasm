OPENQASM 2.0;
include "qelib1.inc";
qreg q[12];
h q[2];
cx q[1],q[2];
tdg q[2];
cx q[0],q[2];
t q[2];
cx q[1],q[2];
tdg q[2];
cx q[0],q[2];
t q[1];
t q[2];
cx q[0],q[1];
h q[2];
t q[0];
tdg q[1];
cx q[0],q[1];
x q[2];
h q[4];
cx q[3],q[4];
tdg q[4];
cx q[1],q[4];
t q[4];
cx q[3],q[4];
tdg q[4];
cx q[1],q[4];
t q[3];
t q[4];
cx q[1],q[3];
h q[4];
t q[1];
tdg q[3];
cx q[1],q[3];
h q[6];
cx q[5],q[6];
tdg q[6];
cx q[2],q[6];
t q[6];
cx q[5],q[6];
tdg q[6];
cx q[2],q[6];
t q[5];
t q[6];
cx q[2],q[5];
h q[6];
t q[2];
tdg q[5];
cx q[2],q[5];
x q[4];
x q[6];
h q[7];
cx q[4],q[7];
tdg q[7];
cx q[6],q[7];
t q[7];
cx q[4],q[7];
tdg q[7];
cx q[6],q[7];
t q[4];
t q[7];
cx q[6],q[4];
h q[7];
t q[6];
tdg q[4];
cx q[6],q[4];
x q[8];
x q[4];
h q[4];
cx q[3],q[4];
tdg q[4];
cx q[1],q[4];
t q[4];
cx q[3],q[4];
tdg q[4];
cx q[1],q[4];
t q[3];
t q[4];
cx q[1],q[3];
h q[4];
t q[1];
tdg q[3];
cx q[1],q[3];
h q[4];
cx q[9],q[4];
tdg q[4];
cx q[2],q[4];
t q[4];
cx q[9],q[4];
tdg q[4];
cx q[2],q[4];
t q[9];
t q[4];
cx q[2],q[9];
h q[4];
t q[2];
tdg q[9];
cx q[2],q[9];
x q[4];
h q[10];
cx q[6],q[10];
tdg q[10];
cx q[4],q[10];
t q[10];
cx q[6],q[10];
tdg q[10];
cx q[4],q[10];
t q[6];
t q[10];
cx q[4],q[6];
h q[10];
t q[4];
tdg q[6];
cx q[4],q[6];
x q[11];
x q[6];
x q[4];
h q[6];
cx q[5],q[6];
tdg q[6];
cx q[2],q[6];
t q[6];
cx q[5],q[6];
tdg q[6];
cx q[2],q[6];
t q[5];
t q[6];
cx q[2],q[5];
h q[6];
t q[2];
tdg q[5];
cx q[2],q[5];
h q[4];
cx q[9],q[4];
tdg q[4];
cx q[2],q[4];
t q[4];
cx q[9],q[4];
tdg q[4];
cx q[2],q[4];
t q[9];
t q[4];
cx q[2],q[9];
h q[4];
t q[2];
tdg q[9];
cx q[2],q[9];
x q[2];
h q[2];
cx q[1],q[2];
tdg q[2];
cx q[0],q[2];
t q[2];
cx q[1],q[2];
tdg q[2];
cx q[0],q[2];
t q[1];
t q[2];
cx q[0],q[1];
h q[2];
t q[0];
tdg q[1];
cx q[0],q[1];
