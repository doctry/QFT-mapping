OPENQASM 2.0;
include "qelib1.inc";
qreg q[201];
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
cx q[3],q[4];
cx q[5],q[6];
cx q[7],q[8];
cx q[9],q[10];
cx q[11],q[12];
cx q[13],q[14];
cx q[15],q[16];
cx q[17],q[18];
cx q[19],q[20];
cx q[21],q[22];
cx q[23],q[24];
cx q[25],q[26];
cx q[27],q[28];
cx q[29],q[4];
cx q[30],q[6];
cx q[2],q[31];
cx q[32],q[8];
x q[10];
cx q[33],q[12];
h q[35];
cx q[34],q[35];
tdg q[35];
cx q[0],q[35];
t q[35];
cx q[34],q[35];
tdg q[35];
cx q[0],q[35];
t q[34];
t q[35];
cx q[0],q[34];
h q[35];
t q[0];
tdg q[34];
cx q[0],q[34];
cx q[36],q[14];
cx q[37],q[16];
cx q[38],q[18];
cx q[39],q[20];
cx q[40],q[22];
cx q[41],q[24];
cx q[42],q[26];
cx q[43],q[28];
cx q[5],q[44];
cx q[27],q[45];
cx q[19],q[46];
cx q[11],q[47];
cx q[3],q[48];
cx q[21],q[49];
cx q[9],q[50];
cx q[4],q[31];
cx q[8],q[51];
cx q[12],q[52];
cx q[35],q[53];
cx q[18],q[54];
cx q[20],q[55];
h q[57];
cx q[56],q[57];
tdg q[57];
cx q[0],q[57];
t q[57];
cx q[56],q[57];
tdg q[57];
cx q[0],q[57];
t q[56];
t q[57];
cx q[0],q[56];
h q[57];
t q[0];
tdg q[56];
cx q[0],q[56];
cx q[30],q[58];
cx q[41],q[59];
cx q[15],q[44];
cx q[23],q[45];
cx q[17],q[46];
cx q[7],q[47];
cx q[13],q[48];
x q[49];
cx q[29],q[60];
cx q[39],q[61];
cx q[33],q[62];
cx q[19],q[63];
cx q[11],q[64];
cx q[3],q[65];
cx q[5],q[66];
cx q[27],q[67];
cx q[6],q[31];
cx q[68],q[51];
cx q[69],q[52];
cx q[14],q[53];
cx q[70],q[54];
cx q[71],q[55];
cx q[18],q[72];
cx q[57],q[73];
cx q[12],q[74];
h q[76];
cx q[75],q[76];
tdg q[76];
cx q[0],q[76];
t q[76];
cx q[75],q[76];
tdg q[76];
cx q[0],q[76];
t q[75];
t q[76];
cx q[0],q[75];
h q[76];
t q[0];
tdg q[75];
cx q[0],q[75];
cx q[37],q[58];
x q[59];
cx q[44],q[77];
cx q[48],q[78];
cx q[36],q[60];
cx q[38],q[61];
cx q[32],q[62];
cx q[17],q[79];
cx q[39],q[80];
cx q[7],q[81];
cx q[33],q[82];
cx q[29],q[83];
cx q[13],q[84];
cx q[30],q[85];
cx q[15],q[86];
cx q[23],q[87];
cx q[10],q[51];
cx q[88],q[52];
cx q[31],q[89];
cx q[16],q[53];
cx q[90],q[54];
cx q[91],q[55];
cx q[70],q[72];
cx q[22],q[73];
cx q[69],q[74];
cx q[76],q[92];
cx q[58],q[93];
cx q[45],q[77];
h q[95];
cx q[94],q[95];
tdg q[95];
cx q[0],q[95];
t q[95];
cx q[94],q[95];
tdg q[95];
cx q[0],q[95];
t q[94];
t q[95];
cx q[0],q[94];
h q[95];
t q[0];
tdg q[94];
cx q[0],q[94];
cx q[25],q[78];
cx q[60],q[96];
cx q[71],q[97];
cx q[68],q[98];
cx q[48],q[99];
cx q[38],q[100];
cx q[32],q[101];
cx q[36],q[102];
cx q[37],q[103];
cx q[51],q[89];
cx q[53],q[104];
cx q[90],q[105];
cx q[24],q[73];
cx q[88],q[106];
cx q[26],q[92];
cx q[43],q[93];
cx q[95],q[107];
cx q[49],q[78];
cx q[42],q[96];
h q[109];
cx q[108],q[109];
tdg q[109];
cx q[0],q[109];
t q[109];
cx q[108],q[109];
tdg q[109];
cx q[0],q[109];
t q[108];
t q[109];
cx q[0],q[108];
h q[109];
t q[0];
tdg q[108];
cx q[0],q[108];
cx q[91],q[110];
cx q[70],q[97];
x q[98];
cx q[25],q[99];
cx q[71],q[111];
cx q[68],q[112];
cx q[52],q[89];
cx q[54],q[104];
x q[105];
cx q[51],q[113];
x q[106];
cx q[28],q[92];
cx q[59],q[93];
cx q[46],q[107];
cx q[40],q[96];
cx q[109],q[114];
cx q[78],q[115];
cx q[90],q[110];
h q[117];
cx q[116],q[117];
tdg q[117];
cx q[0],q[117];
t q[117];
cx q[116],q[117];
tdg q[117];
cx q[0],q[117];
t q[116];
t q[117];
cx q[0],q[116];
h q[117];
t q[0];
tdg q[116];
cx q[0],q[116];
cx q[97],q[118];
cx q[43],q[119];
cx q[21],q[99];
cx q[70],q[120];
cx q[91],q[121];
cx q[53],q[122];
cx q[42],q[123];
cx q[25],q[124];
cx q[55],q[104];
cx q[105],q[72];
cx q[106],q[74];
cx q[47],q[107];
cx q[93],q[125];
cx q[61],q[114];
cx q[96],q[115];
cx q[110],q[126];
cx q[69],q[118];
h q[128];
cx q[127],q[128];
tdg q[128];
cx q[0],q[128];
t q[128];
cx q[127],q[128];
tdg q[128];
cx q[0],q[128];
t q[127];
t q[128];
cx q[0],q[127];
h q[128];
t q[0];
tdg q[127];
cx q[0],q[127];
cx q[41],q[119];
cx q[90],q[129];
cx q[40],q[130];
cx q[21],q[131];
cx q[43],q[132];
h q[133];
cx q[104],q[133];
tdg q[133];
cx q[89],q[133];
t q[133];
cx q[104],q[133];
tdg q[133];
cx q[89],q[133];
t q[104];
t q[133];
cx q[89],q[104];
h q[133];
t q[89];
tdg q[104];
cx q[89],q[104];
cx q[72],q[113];
cx q[74],q[134];
cx q[77],q[125];
cx q[62],q[114];
cx q[88],q[126];
cx q[98],q[118];
cx q[128],q[135];
cx q[69],q[136];
cx q[41],q[137];
cx q[73],q[113];
cx q[55],q[134];
cx q[107],q[125];
cx q[114],q[115];
cx q[10],q[126];
cx q[44],q[135];
cx q[118],q[138];
cx q[72],q[122];
cx q[88],q[139];
cx q[92],q[134];
h q[140];
cx q[115],q[140];
tdg q[140];
cx q[125],q[140];
t q[140];
cx q[115],q[140];
tdg q[140];
cx q[125],q[140];
t q[115];
t q[140];
cx q[125],q[115];
h q[140];
t q[125];
tdg q[115];
cx q[125],q[115];
cx q[126],q[141];
cx q[45],q[135];
cx q[118],q[142];
h q[143];
cx q[134],q[143];
tdg q[143];
cx q[113],q[143];
t q[143];
cx q[134],q[143];
tdg q[143];
cx q[113],q[143];
t q[134];
t q[143];
cx q[113],q[134];
h q[143];
t q[113];
tdg q[134];
cx q[113],q[134];
cx q[96],q[141];
cx q[135],q[138];
cx q[126],q[144];
h q[145];
cx q[143],q[145];
tdg q[145];
cx q[133],q[145];
t q[145];
cx q[143],q[145];
tdg q[145];
cx q[133],q[145];
t q[143];
t q[145];
cx q[133],q[143];
h q[145];
t q[133];
tdg q[143];
cx q[133],q[143];
cx q[141],q[146];
cx q[78],q[138];
cx q[135],q[142];
cx q[117],q[146];
cx q[99],q[142];
cx q[93],q[146];
cx q[117],q[147];
cx q[99],q[148];
h q[149];
cx q[146],q[149];
tdg q[149];
cx q[140],q[149];
t q[149];
cx q[146],q[149];
tdg q[149];
cx q[140],q[149];
t q[146];
t q[149];
cx q[140],q[146];
h q[149];
t q[140];
tdg q[146];
cx q[140],q[146];
cx q[58],q[147];
h q[149];
cx q[138],q[149];
tdg q[149];
cx q[140],q[149];
t q[149];
cx q[138],q[149];
tdg q[149];
cx q[140],q[149];
t q[138];
t q[149];
cx q[140],q[138];
h q[149];
t q[140];
tdg q[138];
cx q[140],q[138];
cx q[119],q[147];
cx q[58],q[150];
x q[149];
cx q[147],q[144];
cx q[119],q[150];
cx q[96],q[144];
cx q[150],q[151];
h q[152];
cx q[142],q[152];
tdg q[152];
cx q[144],q[152];
t q[152];
cx q[142],q[152];
tdg q[152];
cx q[144],q[152];
t q[142];
t q[152];
cx q[144],q[142];
h q[152];
t q[144];
tdg q[142];
cx q[144],q[142];
cx q[77],q[151];
cx q[96],q[148];
cx q[107],q[151];
cx q[114],q[148];
h q[153];
cx q[151],q[153];
tdg q[153];
cx q[152],q[153];
t q[153];
cx q[151],q[153];
tdg q[153];
cx q[152],q[153];
t q[151];
t q[153];
cx q[152],q[151];
h q[153];
t q[152];
tdg q[151];
cx q[152],q[151];
h q[153];
cx q[148],q[153];
tdg q[153];
cx q[152],q[153];
t q[153];
cx q[148],q[153];
tdg q[153];
cx q[152],q[153];
t q[148];
t q[153];
cx q[152],q[148];
h q[153];
t q[152];
tdg q[148];
cx q[152],q[148];
x q[153];
h q[154];
cx q[153],q[154];
tdg q[154];
cx q[149],q[154];
t q[154];
cx q[153],q[154];
tdg q[154];
cx q[149],q[154];
t q[153];
t q[154];
cx q[149],q[153];
h q[154];
t q[149];
tdg q[153];
cx q[149],q[153];
x q[154];
h q[155];
cx q[154],q[155];
tdg q[155];
cx q[145],q[155];
t q[155];
cx q[154],q[155];
tdg q[155];
cx q[145],q[155];
t q[154];
t q[155];
cx q[145],q[154];
h q[155];
t q[145];
tdg q[154];
cx q[145],q[154];
cx q[155],q[100];
h q[156];
cx q[115],q[156];
tdg q[156];
cx q[155],q[156];
t q[156];
cx q[115],q[156];
tdg q[156];
cx q[155],q[156];
t q[115];
t q[156];
cx q[155],q[115];
h q[156];
t q[155];
tdg q[115];
cx q[155],q[115];
cx q[155],q[79];
h q[157];
cx q[125],q[157];
tdg q[157];
cx q[155],q[157];
t q[157];
cx q[125],q[157];
tdg q[157];
cx q[155],q[157];
t q[125];
t q[157];
cx q[155],q[125];
h q[157];
t q[155];
tdg q[125];
cx q[155],q[125];
cx q[155],q[129];
h q[158];
cx q[144],q[158];
tdg q[158];
cx q[155],q[158];
t q[158];
cx q[144],q[158];
tdg q[158];
cx q[155],q[158];
t q[144];
t q[158];
cx q[155],q[144];
h q[158];
t q[155];
tdg q[144];
cx q[155],q[144];
cx q[155],q[120];
h q[159];
cx q[142],q[159];
tdg q[159];
cx q[155],q[159];
t q[159];
cx q[142],q[159];
tdg q[159];
cx q[155],q[159];
t q[142];
t q[159];
cx q[155],q[142];
h q[159];
t q[155];
tdg q[142];
cx q[155],q[142];
h q[155];
cx q[154],q[155];
tdg q[155];
cx q[145],q[155];
t q[155];
cx q[154],q[155];
tdg q[155];
cx q[145],q[155];
t q[154];
t q[155];
cx q[145],q[154];
h q[155];
t q[145];
tdg q[154];
cx q[145],q[154];
x q[154];
cx q[52],q[155];
h q[154];
cx q[153],q[154];
tdg q[154];
cx q[149],q[154];
t q[154];
cx q[153],q[154];
tdg q[154];
cx q[149],q[154];
t q[153];
t q[154];
cx q[149],q[153];
h q[154];
t q[149];
tdg q[153];
cx q[149],q[153];
cx q[55],q[155];
cx q[51],q[154];
cx q[92],q[155];
h q[79];
cx q[153],q[79];
tdg q[79];
cx q[149],q[79];
t q[79];
cx q[153],q[79];
tdg q[79];
cx q[149],q[79];
t q[153];
t q[79];
cx q[149],q[153];
h q[79];
t q[149];
tdg q[153];
cx q[149],q[153];
cx q[55],q[122];
cx q[54],q[154];
x q[160];
h q[161];
cx q[153],q[161];
tdg q[161];
cx q[149],q[161];
t q[161];
cx q[153],q[161];
tdg q[161];
cx q[149],q[161];
t q[153];
t q[161];
cx q[149],q[153];
h q[161];
t q[149];
tdg q[153];
cx q[149],q[153];
cx q[73],q[154];
x q[162];
h q[100];
cx q[155],q[100];
tdg q[100];
cx q[154],q[100];
t q[100];
cx q[155],q[100];
tdg q[100];
cx q[154],q[100];
t q[155];
t q[100];
cx q[154],q[155];
h q[100];
t q[154];
tdg q[155];
cx q[154],q[155];
h q[129];
cx q[79],q[129];
tdg q[129];
cx q[100],q[129];
t q[129];
cx q[79],q[129];
tdg q[129];
cx q[100],q[129];
t q[79];
t q[129];
cx q[100],q[79];
h q[129];
t q[100];
tdg q[79];
cx q[100],q[79];
h q[120];
cx q[133],q[120];
tdg q[120];
cx q[129],q[120];
t q[120];
cx q[133],q[120];
tdg q[120];
cx q[129],q[120];
t q[133];
t q[120];
cx q[129],q[133];
h q[120];
t q[129];
tdg q[133];
cx q[129],q[133];
cx q[161],q[80];
h q[163];
cx q[115],q[163];
tdg q[163];
cx q[120],q[163];
t q[163];
cx q[115],q[163];
tdg q[163];
cx q[120],q[163];
t q[115];
t q[163];
cx q[120],q[115];
h q[163];
t q[120];
tdg q[115];
cx q[120],q[115];
cx q[161],q[63];
h q[164];
cx q[125],q[164];
tdg q[164];
cx q[120],q[164];
t q[164];
cx q[125],q[164];
tdg q[164];
cx q[120],q[164];
t q[125];
t q[164];
cx q[120],q[125];
h q[164];
t q[120];
tdg q[125];
cx q[120],q[125];
cx q[161],q[121];
h q[165];
cx q[144],q[165];
tdg q[165];
cx q[120],q[165];
t q[165];
cx q[144],q[165];
tdg q[165];
cx q[120],q[165];
t q[144];
t q[165];
cx q[120],q[144];
h q[165];
t q[120];
tdg q[144];
cx q[120],q[144];
cx q[161],q[111];
h q[166];
cx q[142],q[166];
tdg q[166];
cx q[120],q[166];
t q[166];
cx q[142],q[166];
tdg q[166];
cx q[120],q[166];
t q[142];
t q[166];
cx q[120],q[142];
h q[166];
t q[120];
tdg q[142];
cx q[120],q[142];
h q[120];
cx q[133],q[120];
tdg q[120];
cx q[129],q[120];
t q[120];
cx q[133],q[120];
tdg q[120];
cx q[129],q[120];
t q[133];
t q[120];
cx q[129],q[133];
h q[120];
t q[129];
tdg q[133];
cx q[129],q[133];
cx q[31],q[161];
cx q[51],q[161];
cx q[74],q[161];
h q[160];
cx q[167],q[160];
tdg q[160];
cx q[120],q[160];
t q[160];
cx q[167],q[160];
tdg q[160];
cx q[120],q[160];
t q[167];
t q[160];
cx q[120],q[167];
h q[160];
t q[120];
tdg q[167];
cx q[120],q[167];
h q[168];
cx q[143],q[168];
tdg q[168];
cx q[160],q[168];
t q[168];
cx q[143],q[168];
tdg q[168];
cx q[160],q[168];
t q[143];
t q[168];
cx q[160],q[143];
h q[168];
t q[160];
tdg q[143];
cx q[160],q[143];
h q[80];
cx q[161],q[80];
tdg q[80];
cx q[168],q[80];
t q[80];
cx q[161],q[80];
tdg q[80];
cx q[168],q[80];
t q[161];
t q[80];
cx q[168],q[161];
h q[80];
t q[168];
tdg q[161];
cx q[168],q[161];
h q[63];
cx q[129],q[63];
tdg q[63];
cx q[160],q[63];
t q[63];
cx q[129],q[63];
tdg q[63];
cx q[160],q[63];
t q[129];
t q[63];
cx q[160],q[129];
h q[63];
t q[160];
tdg q[129];
cx q[160],q[129];
h q[145];
cx q[143],q[145];
tdg q[145];
cx q[133],q[145];
t q[145];
cx q[143],q[145];
tdg q[145];
cx q[133],q[145];
t q[143];
t q[145];
cx q[133],q[143];
h q[145];
t q[133];
tdg q[143];
cx q[133],q[143];
cx q[169],q[101];
cx q[170],q[82];
h q[171];
cx q[115],q[171];
tdg q[171];
cx q[80],q[171];
t q[171];
cx q[115],q[171];
tdg q[171];
cx q[80],q[171];
t q[115];
t q[171];
cx q[80],q[115];
h q[171];
t q[80];
tdg q[115];
cx q[80],q[115];
cx q[169],q[81];
h q[172];
cx q[115],q[172];
tdg q[172];
cx q[63],q[172];
t q[172];
cx q[115],q[172];
tdg q[172];
cx q[63],q[172];
t q[115];
t q[172];
cx q[63],q[115];
h q[172];
t q[63];
tdg q[115];
cx q[63],q[115];
h q[173];
cx q[125],q[173];
tdg q[173];
cx q[80],q[173];
t q[173];
cx q[125],q[173];
tdg q[173];
cx q[80],q[173];
t q[125];
t q[173];
cx q[80],q[125];
h q[173];
t q[80];
tdg q[125];
cx q[80],q[125];
cx q[170],q[64];
cx q[169],q[50];
h q[174];
cx q[125],q[174];
tdg q[174];
cx q[63],q[174];
t q[174];
cx q[125],q[174];
tdg q[174];
cx q[63],q[174];
t q[125];
t q[174];
cx q[63],q[125];
h q[174];
t q[63];
tdg q[125];
cx q[63],q[125];
h q[175];
cx q[144],q[175];
tdg q[175];
cx q[80],q[175];
t q[175];
cx q[144],q[175];
tdg q[175];
cx q[80],q[175];
t q[144];
t q[175];
cx q[80],q[144];
h q[175];
t q[80];
tdg q[144];
cx q[80],q[144];
cx q[170],q[139];
cx q[169],q[112];
h q[176];
cx q[144],q[176];
tdg q[176];
cx q[63],q[176];
t q[176];
cx q[144],q[176];
tdg q[176];
cx q[63],q[176];
t q[144];
t q[176];
cx q[63],q[144];
h q[176];
t q[63];
tdg q[144];
cx q[63],q[144];
h q[177];
cx q[142],q[177];
tdg q[177];
cx q[80],q[177];
t q[177];
cx q[142],q[177];
tdg q[177];
cx q[80],q[177];
t q[142];
t q[177];
cx q[80],q[142];
h q[177];
t q[80];
tdg q[142];
cx q[80],q[142];
cx q[170],q[136];
h q[178];
cx q[142],q[178];
tdg q[178];
cx q[63],q[178];
t q[178];
cx q[142],q[178];
tdg q[178];
cx q[63],q[178];
t q[142];
t q[178];
cx q[63],q[142];
h q[178];
t q[63];
tdg q[142];
cx q[63],q[142];
h q[80];
cx q[161],q[80];
tdg q[80];
cx q[168],q[80];
t q[80];
cx q[161],q[80];
tdg q[80];
cx q[168],q[80];
t q[161];
t q[80];
cx q[168],q[161];
h q[80];
t q[168];
tdg q[161];
cx q[168],q[161];
h q[63];
cx q[129],q[63];
tdg q[63];
cx q[160],q[63];
t q[63];
cx q[129],q[63];
tdg q[63];
cx q[160],q[63];
t q[129];
t q[63];
cx q[160],q[129];
h q[63];
t q[160];
tdg q[129];
cx q[160],q[129];
x q[162];
h q[129];
cx q[79],q[129];
tdg q[129];
cx q[100],q[129];
t q[129];
cx q[79],q[129];
tdg q[129];
cx q[100],q[129];
t q[79];
t q[129];
cx q[100],q[79];
h q[129];
t q[100];
tdg q[79];
cx q[100],q[79];
h q[168];
cx q[143],q[168];
tdg q[168];
cx q[160],q[168];
t q[168];
cx q[143],q[168];
tdg q[168];
cx q[160],q[168];
t q[143];
t q[168];
cx q[160],q[143];
h q[168];
t q[160];
tdg q[143];
cx q[160],q[143];
x q[160];
h q[79];
cx q[153],q[79];
tdg q[79];
cx q[149],q[79];
t q[79];
cx q[153],q[79];
tdg q[79];
cx q[149],q[79];
t q[153];
t q[79];
cx q[149],q[153];
h q[79];
t q[149];
tdg q[153];
cx q[149],q[153];
h q[161];
cx q[153],q[161];
tdg q[161];
cx q[149],q[161];
t q[161];
cx q[153],q[161];
tdg q[161];
cx q[149],q[161];
t q[153];
t q[161];
cx q[149],q[153];
h q[161];
t q[149];
tdg q[153];
cx q[149],q[153];
h q[79];
cx q[167],q[79];
tdg q[79];
cx q[89],q[79];
t q[79];
cx q[167],q[79];
tdg q[79];
cx q[89],q[79];
t q[167];
t q[79];
cx q[89],q[167];
h q[79];
t q[89];
tdg q[167];
cx q[89],q[167];
x q[149];
h q[129];
cx q[113],q[129];
tdg q[129];
cx q[79],q[129];
t q[129];
cx q[113],q[129];
tdg q[129];
cx q[79],q[129];
t q[113];
t q[129];
cx q[79],q[113];
h q[129];
t q[79];
tdg q[113];
cx q[79],q[113];
x q[153];
h q[149];
cx q[146],q[149];
tdg q[149];
cx q[140],q[149];
t q[149];
cx q[146],q[149];
tdg q[149];
cx q[140],q[149];
t q[146];
t q[149];
cx q[140],q[146];
h q[149];
t q[140];
tdg q[146];
cx q[140],q[146];
h q[129];
cx q[155],q[129];
tdg q[129];
cx q[79],q[129];
t q[129];
cx q[155],q[129];
tdg q[129];
cx q[79],q[129];
t q[155];
t q[129];
cx q[79],q[155];
h q[129];
t q[79];
tdg q[155];
cx q[79],q[155];
h q[149];
cx q[138],q[149];
tdg q[149];
cx q[140],q[149];
t q[149];
cx q[138],q[149];
tdg q[149];
cx q[140],q[149];
t q[138];
t q[149];
cx q[140],q[138];
h q[149];
t q[140];
tdg q[138];
cx q[140],q[138];
x q[168];
h q[121];
cx q[142],q[121];
tdg q[121];
cx q[146],q[121];
t q[121];
cx q[142],q[121];
tdg q[121];
cx q[146],q[121];
t q[142];
t q[121];
cx q[146],q[142];
h q[121];
t q[146];
tdg q[142];
cx q[146],q[142];
h q[140];
cx q[115],q[140];
tdg q[140];
cx q[125],q[140];
t q[140];
cx q[115],q[140];
tdg q[140];
cx q[125],q[140];
t q[115];
t q[140];
cx q[125],q[115];
h q[140];
t q[125];
tdg q[115];
cx q[125],q[115];
h q[149];
cx q[134],q[149];
tdg q[149];
cx q[154],q[149];
t q[149];
cx q[134],q[149];
tdg q[149];
cx q[154],q[149];
t q[134];
t q[149];
cx q[154],q[134];
h q[149];
t q[154];
tdg q[134];
cx q[154],q[134];
h q[122];
cx q[138],q[122];
tdg q[122];
cx q[144],q[122];
t q[122];
cx q[138],q[122];
tdg q[122];
cx q[144],q[122];
t q[138];
t q[122];
cx q[144],q[138];
h q[122];
t q[144];
tdg q[138];
cx q[144],q[138];
h q[140];
cx q[148],q[140];
tdg q[140];
cx q[125],q[140];
t q[140];
cx q[148],q[140];
tdg q[140];
cx q[125],q[140];
t q[148];
t q[140];
cx q[125],q[148];
h q[140];
t q[125];
tdg q[148];
cx q[125],q[148];
h q[161];
cx q[120],q[161];
tdg q[161];
cx q[149],q[161];
t q[161];
cx q[120],q[161];
tdg q[161];
cx q[149],q[161];
t q[120];
t q[161];
cx q[149],q[120];
h q[161];
t q[149];
tdg q[120];
cx q[149],q[120];
h q[179];
cx q[115],q[179];
tdg q[179];
cx q[151],q[179];
t q[179];
cx q[115],q[179];
tdg q[179];
cx q[151],q[179];
t q[115];
t q[179];
cx q[151],q[115];
h q[179];
t q[151];
tdg q[115];
cx q[151],q[115];
h q[161];
cx q[104],q[161];
tdg q[161];
cx q[149],q[161];
t q[161];
cx q[104],q[161];
tdg q[161];
cx q[149],q[161];
t q[104];
t q[161];
cx q[149],q[104];
h q[161];
t q[149];
tdg q[104];
cx q[149],q[104];
x q[162];
h q[63];
cx q[129],q[63];
tdg q[63];
cx q[161],q[63];
t q[63];
cx q[129],q[63];
tdg q[63];
cx q[161],q[63];
t q[129];
t q[63];
cx q[161],q[129];
h q[63];
t q[161];
tdg q[129];
cx q[161],q[129];
x q[170];
h q[162];
cx q[129],q[162];
tdg q[162];
cx q[161],q[162];
t q[162];
cx q[129],q[162];
tdg q[162];
cx q[161],q[162];
t q[129];
t q[162];
cx q[161],q[129];
h q[162];
t q[161];
tdg q[129];
cx q[161],q[129];
h q[80];
cx q[63],q[80];
tdg q[80];
cx q[140],q[80];
t q[80];
cx q[63],q[80];
tdg q[80];
cx q[140],q[80];
t q[63];
t q[80];
cx q[140],q[63];
h q[80];
t q[140];
tdg q[63];
cx q[140],q[63];
x q[180];
h q[111];
cx q[121],q[111];
tdg q[111];
cx q[80],q[111];
t q[111];
cx q[121],q[111];
tdg q[111];
cx q[80],q[111];
t q[121];
t q[111];
cx q[80],q[121];
h q[111];
t q[80];
tdg q[121];
cx q[80],q[121];
h q[169];
cx q[162],q[169];
tdg q[169];
cx q[179],q[169];
t q[169];
cx q[162],q[169];
tdg q[169];
cx q[179],q[169];
t q[162];
t q[169];
cx q[179],q[162];
h q[169];
t q[179];
tdg q[162];
cx q[179],q[162];
cx q[181],q[102];
h q[182];
cx q[122],q[182];
tdg q[182];
cx q[80],q[182];
t q[182];
cx q[122],q[182];
tdg q[182];
cx q[80],q[182];
t q[122];
t q[182];
cx q[80],q[122];
h q[182];
t q[80];
tdg q[122];
cx q[80],q[122];
h q[101];
cx q[121],q[101];
tdg q[101];
cx q[169],q[101];
t q[101];
cx q[121],q[101];
tdg q[101];
cx q[169],q[101];
t q[121];
t q[101];
cx q[169],q[121];
h q[101];
t q[169];
tdg q[121];
cx q[169],q[121];
h q[183];
cx q[167],q[183];
tdg q[183];
cx q[111],q[183];
t q[183];
cx q[167],q[183];
tdg q[183];
cx q[111],q[183];
t q[167];
t q[183];
cx q[111],q[167];
h q[183];
t q[111];
tdg q[167];
cx q[111],q[167];
cx q[184],q[84];
cx q[185],q[103];
cx q[181],q[83];
h q[186];
cx q[167],q[186];
tdg q[186];
cx q[182],q[186];
t q[186];
cx q[167],q[186];
tdg q[186];
cx q[182],q[186];
t q[167];
t q[186];
cx q[182],q[167];
h q[186];
t q[182];
tdg q[167];
cx q[182],q[167];
h q[187];
cx q[89],q[187];
tdg q[187];
cx q[111],q[187];
t q[187];
cx q[89],q[187];
tdg q[187];
cx q[111],q[187];
t q[89];
t q[187];
cx q[111],q[89];
h q[187];
t q[111];
tdg q[89];
cx q[111],q[89];
cx q[184],q[65];
h q[188];
cx q[167],q[188];
tdg q[188];
cx q[101],q[188];
t q[188];
cx q[167],q[188];
tdg q[188];
cx q[101],q[188];
t q[167];
t q[188];
cx q[101],q[167];
h q[188];
t q[101];
tdg q[167];
cx q[101],q[167];
cx q[181],q[130];
h q[189];
cx q[89],q[189];
tdg q[189];
cx q[182],q[189];
t q[189];
cx q[89],q[189];
tdg q[189];
cx q[182],q[189];
t q[89];
t q[189];
cx q[182],q[89];
h q[189];
t q[182];
tdg q[89];
cx q[182],q[89];
cx q[185],q[85];
h q[190];
cx q[154],q[190];
tdg q[190];
cx q[111],q[190];
t q[190];
cx q[154],q[190];
tdg q[190];
cx q[111],q[190];
t q[154];
t q[190];
cx q[111],q[154];
h q[190];
t q[111];
tdg q[154];
cx q[111],q[154];
cx q[184],q[131];
h q[191];
cx q[89],q[191];
tdg q[191];
cx q[101],q[191];
t q[191];
cx q[89],q[191];
tdg q[191];
cx q[101],q[191];
t q[89];
t q[191];
cx q[101],q[89];
h q[191];
t q[101];
tdg q[89];
cx q[101],q[89];
cx q[181],q[123];
h q[192];
cx q[154],q[192];
tdg q[192];
cx q[182],q[192];
t q[192];
cx q[154],q[192];
tdg q[192];
cx q[182],q[192];
t q[154];
t q[192];
cx q[182],q[154];
h q[192];
t q[182];
tdg q[154];
cx q[182],q[154];
cx q[185],q[137];
h q[193];
cx q[134],q[193];
tdg q[193];
cx q[111],q[193];
t q[193];
cx q[134],q[193];
tdg q[193];
cx q[111],q[193];
t q[134];
t q[193];
cx q[111],q[134];
h q[193];
t q[111];
tdg q[134];
cx q[111],q[134];
cx q[184],q[124];
h q[194];
cx q[154],q[194];
tdg q[194];
cx q[101],q[194];
t q[194];
cx q[154],q[194];
tdg q[194];
cx q[101],q[194];
t q[154];
t q[194];
cx q[101],q[154];
h q[194];
t q[101];
tdg q[154];
cx q[101],q[154];
h q[195];
cx q[134],q[195];
tdg q[195];
cx q[182],q[195];
t q[195];
cx q[134],q[195];
tdg q[195];
cx q[182],q[195];
t q[134];
t q[195];
cx q[182],q[134];
h q[195];
t q[182];
tdg q[134];
cx q[182],q[134];
cx q[185],q[132];
h q[196];
cx q[134],q[196];
tdg q[196];
cx q[101],q[196];
t q[196];
cx q[134],q[196];
tdg q[196];
cx q[101],q[196];
t q[134];
t q[196];
cx q[101],q[134];
h q[196];
t q[101];
tdg q[134];
cx q[101],q[134];
h q[101];
cx q[121],q[101];
tdg q[101];
cx q[169],q[101];
t q[101];
cx q[121],q[101];
tdg q[101];
cx q[169],q[101];
t q[121];
t q[101];
cx q[169],q[121];
h q[101];
t q[169];
tdg q[121];
cx q[169],q[121];
h q[101];
cx q[122],q[101];
tdg q[101];
cx q[169],q[101];
t q[101];
cx q[122],q[101];
tdg q[101];
cx q[169],q[101];
t q[122];
t q[101];
cx q[169],q[122];
h q[101];
t q[169];
tdg q[122];
cx q[169],q[122];
h q[111];
cx q[121],q[111];
tdg q[111];
cx q[80],q[111];
t q[111];
cx q[121],q[111];
tdg q[111];
cx q[80],q[111];
t q[121];
t q[111];
cx q[80],q[121];
h q[111];
t q[80];
tdg q[121];
cx q[80],q[121];
cx q[185],q[86];
h q[197];
cx q[167],q[197];
tdg q[197];
cx q[101],q[197];
t q[197];
cx q[167],q[197];
tdg q[197];
cx q[101],q[197];
t q[167];
t q[197];
cx q[101],q[167];
h q[197];
t q[101];
tdg q[167];
cx q[101],q[167];
cx q[185],q[66];
h q[160];
cx q[167],q[160];
tdg q[160];
cx q[120],q[160];
t q[160];
cx q[167],q[160];
tdg q[160];
cx q[120],q[160];
t q[167];
t q[160];
cx q[120],q[167];
h q[160];
t q[120];
tdg q[167];
cx q[120],q[167];
h q[198];
cx q[89],q[198];
tdg q[198];
cx q[101],q[198];
t q[198];
cx q[89],q[198];
tdg q[198];
cx q[101],q[198];
t q[89];
t q[198];
cx q[101],q[89];
h q[198];
t q[101];
tdg q[89];
cx q[101],q[89];
cx q[185],q[87];
h q[199];
cx q[154],q[199];
tdg q[199];
cx q[101],q[199];
t q[199];
cx q[154],q[199];
tdg q[199];
cx q[101],q[199];
t q[154];
t q[199];
cx q[101],q[154];
h q[199];
t q[101];
tdg q[154];
cx q[101],q[154];
cx q[185],q[67];
h q[200];
cx q[134],q[200];
tdg q[200];
cx q[101],q[200];
t q[200];
cx q[134],q[200];
tdg q[200];
cx q[101],q[200];
t q[134];
t q[200];
cx q[101],q[134];
h q[200];
t q[101];
tdg q[134];
cx q[101],q[134];
h q[101];
cx q[122],q[101];
tdg q[101];
cx q[169],q[101];
t q[101];
cx q[122],q[101];
tdg q[101];
cx q[169],q[101];
t q[122];
t q[101];
cx q[169],q[122];
h q[101];
t q[169];
tdg q[122];
cx q[169],q[122];
h q[143];
cx q[134],q[143];
tdg q[143];
cx q[113],q[143];
t q[143];
cx q[134],q[143];
tdg q[143];
cx q[113],q[143];
t q[134];
t q[143];
cx q[113],q[134];
h q[143];
t q[113];
tdg q[134];
cx q[113],q[134];
h q[169];
cx q[162],q[169];
tdg q[169];
cx q[179],q[169];
t q[169];
cx q[162],q[169];
tdg q[169];
cx q[179],q[169];
t q[162];
t q[169];
cx q[179],q[162];
h q[169];
t q[179];
tdg q[162];
cx q[179],q[162];
h q[182];
cx q[122],q[182];
tdg q[182];
cx q[80],q[182];
t q[182];
cx q[122],q[182];
tdg q[182];
cx q[80],q[182];
t q[122];
t q[182];
cx q[80],q[122];
h q[182];
t q[80];
tdg q[122];
cx q[80],q[122];
x q[180];
h q[179];
cx q[115],q[179];
tdg q[179];
cx q[151],q[179];
t q[179];
cx q[115],q[179];
tdg q[179];
cx q[151],q[179];
t q[115];
t q[179];
cx q[151],q[115];
h q[179];
t q[151];
tdg q[115];
cx q[151],q[115];
h q[80];
cx q[63],q[80];
tdg q[80];
cx q[140],q[80];
t q[80];
cx q[63],q[80];
tdg q[80];
cx q[140],q[80];
t q[63];
t q[80];
cx q[140],q[63];
h q[80];
t q[140];
tdg q[63];
cx q[140],q[63];
h q[162];
cx q[129],q[162];
tdg q[162];
cx q[161],q[162];
t q[162];
cx q[129],q[162];
tdg q[162];
cx q[161],q[162];
t q[129];
t q[162];
cx q[161],q[129];
h q[162];
t q[161];
tdg q[129];
cx q[161],q[129];
cx q[78],q[115];
h q[153];
cx q[151],q[153];
tdg q[153];
cx q[152],q[153];
t q[153];
cx q[151],q[153];
tdg q[153];
cx q[152],q[153];
t q[151];
t q[153];
cx q[152],q[151];
h q[153];
t q[152];
tdg q[151];
cx q[152],q[151];
x q[170];
cx q[96],q[115];
h q[153];
cx q[148],q[153];
tdg q[153];
cx q[152],q[153];
t q[153];
cx q[148],q[153];
tdg q[153];
cx q[152],q[153];
t q[148];
t q[153];
cx q[152],q[148];
h q[153];
t q[152];
tdg q[148];
cx q[152],q[148];
h q[63];
cx q[129],q[63];
tdg q[63];
cx q[161],q[63];
t q[63];
cx q[129],q[63];
tdg q[63];
cx q[161],q[63];
t q[129];
t q[63];
cx q[161],q[129];
h q[63];
t q[161];
tdg q[129];
cx q[161],q[129];
cx q[150],q[151];
cx q[114],q[115];
h q[140];
cx q[148],q[140];
tdg q[140];
cx q[125],q[140];
t q[140];
cx q[148],q[140];
tdg q[140];
cx q[125],q[140];
t q[148];
t q[140];
cx q[125],q[148];
h q[140];
t q[125];
tdg q[148];
cx q[125],q[148];
x q[168];
x q[162];
cx q[77],q[151];
h q[152];
cx q[142],q[152];
tdg q[152];
cx q[144],q[152];
t q[152];
cx q[142],q[152];
tdg q[152];
cx q[144],q[152];
t q[142];
t q[152];
cx q[144],q[142];
h q[152];
t q[144];
tdg q[142];
cx q[144],q[142];
cx q[58],q[150];
cx q[99],q[148];
h q[129];
cx q[113],q[129];
tdg q[129];
cx q[79],q[129];
t q[129];
cx q[113],q[129];
tdg q[129];
cx q[79],q[129];
t q[113];
t q[129];
cx q[79],q[113];
h q[129];
t q[79];
tdg q[113];
cx q[79],q[113];
h q[161];
cx q[120],q[161];
tdg q[161];
cx q[149],q[161];
t q[161];
cx q[120],q[161];
tdg q[161];
cx q[149],q[161];
t q[120];
t q[161];
cx q[149],q[120];
h q[161];
t q[149];
tdg q[120];
cx q[149],q[120];
cx q[107],q[151];
cx q[93],q[125];
h q[122];
cx q[138],q[122];
tdg q[122];
cx q[144],q[122];
t q[122];
cx q[138],q[122];
tdg q[122];
cx q[144],q[122];
t q[138];
t q[122];
cx q[144],q[138];
h q[122];
t q[144];
tdg q[138];
cx q[144],q[138];
h q[121];
cx q[142],q[121];
tdg q[121];
cx q[146],q[121];
t q[121];
cx q[142],q[121];
tdg q[121];
cx q[146],q[121];
t q[142];
t q[121];
cx q[146],q[142];
h q[121];
t q[146];
tdg q[142];
cx q[146],q[142];
cx q[119],q[150];
cx q[96],q[148];
h q[129];
cx q[155],q[129];
tdg q[129];
cx q[79],q[129];
t q[129];
cx q[155],q[129];
tdg q[129];
cx q[79],q[129];
t q[155];
t q[129];
cx q[79],q[155];
h q[129];
t q[79];
tdg q[155];
cx q[79],q[155];
cx q[51],q[113];
cx q[77],q[125];
cx q[126],q[144];
cx q[141],q[146];
cx q[118],q[142];
cx q[114],q[148];
cx q[72],q[113];
h q[79];
cx q[167],q[79];
tdg q[79];
cx q[89],q[79];
t q[79];
cx q[167],q[79];
tdg q[79];
cx q[89],q[79];
t q[167];
t q[79];
cx q[89],q[167];
h q[79];
t q[89];
tdg q[167];
cx q[89],q[167];
h q[100];
cx q[155],q[100];
tdg q[100];
cx q[154],q[100];
t q[100];
cx q[155],q[100];
tdg q[100];
cx q[154],q[100];
t q[155];
t q[100];
cx q[154],q[155];
h q[100];
t q[154];
tdg q[155];
cx q[154],q[155];
cx q[107],q[125];
cx q[147],q[144];
cx q[117],q[146];
cx q[126],q[141];
cx q[135],q[142];
cx q[118],q[138];
cx q[44],q[77];
cx q[109],q[114];
cx q[73],q[113];
cx q[53],q[122];
h q[133];
cx q[104],q[133];
tdg q[133];
cx q[89],q[133];
t q[133];
cx q[104],q[133];
tdg q[133];
cx q[89],q[133];
t q[104];
t q[133];
cx q[89],q[104];
h q[133];
t q[89];
tdg q[104];
cx q[89],q[104];
cx q[95],q[107];
cx q[96],q[144];
cx q[93],q[146];
cx q[110],q[126];
cx q[99],q[142];
cx q[135],q[138];
cx q[97],q[118];
cx q[52],q[155];
cx q[117],q[147];
cx q[45],q[77];
cx q[61],q[114];
cx q[72],q[122];
h q[161];
cx q[104],q[161];
tdg q[161];
cx q[149],q[161];
t q[161];
cx q[104],q[161];
tdg q[161];
cx q[149],q[161];
t q[104];
t q[161];
cx q[149],q[104];
h q[161];
t q[149];
tdg q[104];
cx q[149],q[104];
cx q[46],q[107];
cx q[96],q[141];
cx q[88],q[126];
cx q[91],q[110];
cx q[78],q[138];
cx q[69],q[118];
cx q[71],q[97];
cx q[31],q[89];
cx q[58],q[147];
cx q[128],q[135];
h q[109];
cx q[108],q[109];
tdg q[109];
cx q[0],q[109];
t q[109];
cx q[108],q[109];
tdg q[109];
cx q[0],q[109];
t q[108];
t q[109];
cx q[0],q[108];
h q[109];
t q[0];
tdg q[108];
cx q[0],q[108];
cx q[62],q[114];
cx q[39],q[61];
cx q[55],q[122];
cx q[18],q[72];
cx q[53],q[104];
h q[149];
cx q[134],q[149];
tdg q[149];
cx q[154],q[149];
t q[149];
cx q[134],q[149];
tdg q[149];
cx q[154],q[149];
t q[134];
t q[149];
cx q[154],q[134];
h q[149];
t q[154];
tdg q[134];
cx q[154],q[134];
cx q[47],q[107];
cx q[19],q[46];
cx q[10],q[126];
cx q[98],q[118];
cx q[31],q[161];
cx q[60],q[96];
cx q[48],q[78];
cx q[119],q[147];
cx q[58],q[93];
cx q[44],q[135];
h q[95];
cx q[94],q[95];
tdg q[95];
cx q[0],q[95];
t q[95];
cx q[94],q[95];
tdg q[95];
cx q[0],q[95];
t q[94];
t q[95];
cx q[0],q[94];
h q[95];
t q[0];
tdg q[94];
cx q[0],q[94];
cx q[38],q[61];
cx q[70],q[72];
cx q[54],q[104];
cx q[51],q[154];
cx q[74],q[134];
cx q[33],q[62];
cx q[11],q[47];
x q[98];
cx q[42],q[96];
cx q[29],q[60];
cx q[35],q[53];
cx q[2],q[31];
cx q[48],q[99];
cx q[43],q[93];
cx q[30],q[58];
cx q[45],q[135];
h q[117];
cx q[116],q[117];
tdg q[117];
cx q[0],q[117];
t q[117];
cx q[116],q[117];
tdg q[117];
cx q[0],q[117];
t q[116];
t q[117];
cx q[0],q[116];
h q[117];
t q[0];
tdg q[116];
cx q[0],q[116];
cx q[105],q[72];
cx q[55],q[104];
cx q[54],q[154];
cx q[32],q[62];
cx q[51],q[89];
cx q[40],q[96];
cx q[36],q[60];
cx q[14],q[53];
cx q[4],q[31];
cx q[59],q[93];
cx q[37],q[58];
cx q[43],q[119];
h q[128];
cx q[127],q[128];
tdg q[128];
cx q[0],q[128];
t q[128];
cx q[127],q[128];
tdg q[128];
cx q[0],q[128];
t q[127];
t q[128];
cx q[0],q[127];
h q[128];
t q[0];
tdg q[127];
cx q[0],q[127];
cx q[73],q[154];
cx q[18],q[54];
x q[105];
cx q[55],q[134];
cx q[52],q[89];
cx q[51],q[161];
cx q[16],q[53];
cx q[13],q[14];
cx q[6],q[31];
cx q[3],q[4];
cx q[41],q[119];
x q[59];
h q[35];
cx q[34],q[35];
tdg q[35];
cx q[0],q[35];
t q[35];
cx q[34],q[35];
tdg q[35];
cx q[0],q[35];
t q[34];
t q[35];
cx q[0],q[34];
h q[35];
t q[0];
tdg q[34];
cx q[0],q[34];
cx q[70],q[54];
cx q[17],q[18];
cx q[92],q[134];
cx q[55],q[155];
cx q[74],q[161];
cx q[8],q[51];
cx q[36],q[14];
cx q[29],q[4];
cx q[57],q[73];
cx q[3],q[48];
cx q[15],q[16];
cx q[5],q[6];
cx q[41],q[59];
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
cx q[90],q[54];
cx q[38],q[18];
cx q[17],q[46];
cx q[70],q[97];
cx q[92],q[155];
cx q[20],q[55];
cx q[68],q[51];
cx q[7],q[8];
cx q[12],q[74];
cx q[22],q[73];
cx q[13],q[48];
cx q[37],q[16];
cx q[30],q[6];
cx q[5],q[44];
h q[57];
cx q[56],q[57];
tdg q[57];
cx q[0],q[57];
t q[57];
cx q[56],q[57];
tdg q[57];
cx q[0],q[57];
t q[56];
t q[57];
cx q[0],q[56];
h q[57];
t q[0];
tdg q[56];
cx q[0],q[56];
cx q[90],q[110];
cx q[71],q[55];
cx q[19],q[20];
cx q[10],q[51];
cx q[32],q[8];
cx q[7],q[47];
cx q[68],q[98];
cx q[69],q[74];
cx q[12],q[52];
cx q[24],q[73];
cx q[21],q[22];
cx q[76],q[92];
cx q[15],q[44];
cx q[90],q[105];
cx q[91],q[55];
cx q[39],q[20];
x q[10];
cx q[106],q[74];
cx q[69],q[52];
cx q[11],q[12];
cx q[40],q[22];
cx q[26],q[92];
cx q[23],q[24];
h q[76];
cx q[75],q[76];
tdg q[76];
cx q[0],q[76];
t q[76];
cx q[75],q[76];
tdg q[76];
cx q[0],q[76];
t q[75];
t q[76];
cx q[0],q[75];
h q[76];
t q[0];
tdg q[75];
cx q[0],q[75];
cx q[9],q[10];
cx q[88],q[52];
cx q[33],q[12];
x q[106];
cx q[28],q[92];
cx q[25],q[26];
cx q[41],q[24];
cx q[88],q[106];
cx q[42],q[26];
cx q[25],q[78];
cx q[27],q[28];
cx q[49],q[78];
cx q[25],q[99];
cx q[43],q[28];
cx q[27],q[45];
cx q[21],q[99];
x q[49];
cx q[23],q[45];
cx q[21],q[49];