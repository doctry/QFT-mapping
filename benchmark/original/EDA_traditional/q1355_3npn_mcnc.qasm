OPENQASM 2.0;
include "qelib1.inc";
qreg q[195];
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
cx q[29],q[30];
cx q[31],q[4];
cx q[32],q[6];
cx q[2],q[33];
cx q[34],q[8];
x q[10];
cx q[35],q[12];
x q[14];
h q[37];
cx q[36],q[37];
tdg q[37];
cx q[0],q[37];
t q[37];
cx q[36],q[37];
tdg q[37];
cx q[0],q[37];
t q[36];
t q[37];
cx q[0],q[36];
h q[37];
t q[0];
tdg q[36];
cx q[0],q[36];
cx q[38],q[16];
cx q[39],q[18];
cx q[40],q[20];
x q[22];
cx q[41],q[24];
cx q[42],q[26];
cx q[43],q[28];
cx q[44],q[30];
cx q[9],q[45];
cx q[15],q[46];
cx q[7],q[47];
cx q[27],q[48];
cx q[17],q[49];
cx q[13],q[50];
cx q[4],q[33];
cx q[8],q[51];
cx q[12],q[52];
cx q[37],q[53];
cx q[20],q[54];
cx q[24],q[55];
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
cx q[28],q[58];
cx q[30],q[59];
cx q[21],q[45];
cx q[23],q[46];
cx q[34],q[60];
cx q[44],q[61];
cx q[19],q[47];
x q[48];
cx q[38],q[62];
cx q[25],q[49];
cx q[39],q[63];
cx q[15],q[64];
cx q[17],q[65];
cx q[7],q[66];
cx q[9],q[67];
cx q[6],q[33];
cx q[68],q[51];
cx q[69],q[52];
cx q[16],q[53];
cx q[70],q[54];
cx q[26],q[55];
cx q[24],q[71];
cx q[72],q[58];
h q[74];
cx q[73],q[74];
tdg q[74];
cx q[0],q[74];
t q[74];
cx q[73],q[74];
tdg q[74];
cx q[0],q[74];
t q[73];
t q[74];
cx q[0],q[73];
h q[74];
t q[0];
tdg q[73];
cx q[0],q[73];
cx q[4],q[75];
cx q[76],q[59];
cx q[45],q[77];
cx q[46],q[78];
cx q[40],q[60];
cx q[43],q[61];
cx q[47],q[79];
cx q[41],q[62];
cx q[49],q[80];
cx q[42],q[63];
cx q[23],q[81];
cx q[25],q[82];
cx q[38],q[83];
cx q[39],q[84];
cx q[34],q[85];
cx q[44],q[86];
cx q[19],q[87];
cx q[21],q[88];
cx q[10],q[51];
cx q[14],q[52];
cx q[33],q[89];
cx q[18],q[53];
cx q[22],q[54];
cx q[26],q[71];
cx q[90],q[58];
cx q[16],q[91];
cx q[6],q[75];
cx q[92],q[59];
cx q[68],q[93];
cx q[76],q[94];
h q[96];
cx q[95],q[96];
tdg q[96];
cx q[0],q[96];
t q[96];
cx q[95],q[96];
tdg q[96];
cx q[0],q[96];
t q[95];
t q[96];
cx q[0],q[95];
h q[96];
t q[0];
tdg q[95];
cx q[0],q[95];
cx q[3],q[78];
cx q[60],q[97];
cx q[29],q[79];
cx q[62],q[98];
cx q[69],q[99];
cx q[5],q[80];
cx q[63],q[100];
cx q[45],q[101];
cx q[41],q[102];
cx q[42],q[103];
cx q[40],q[104];
cx q[43],q[105];
cx q[51],q[89];
cx q[53],q[106];
cx q[57],q[71];
cx q[18],q[91];
cx q[70],q[93];
cx q[72],q[94];
cx q[90],q[107];
cx q[92],q[77];
cx q[11],q[78];
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
cx q[61],q[97];
cx q[48],q[79];
cx q[31],q[98];
cx q[29],q[110];
x q[99];
cx q[32],q[100];
cx q[69],q[111];
cx q[3],q[112];
cx q[5],q[113];
cx q[33],q[114];
cx q[68],q[115];
cx q[76],q[116];
cx q[52],q[89];
cx q[54],q[106];
cx q[71],q[117];
cx q[74],q[91];
cx q[93],q[118];
x q[107];
cx q[109],q[97];
cx q[35],q[98];
h q[120];
cx q[119],q[120];
tdg q[120];
cx q[0],q[120];
t q[120];
cx q[119],q[120];
tdg q[120];
cx q[0],q[120];
t q[119];
t q[120];
cx q[0],q[119];
h q[120];
t q[0];
tdg q[119];
cx q[0],q[119];
cx q[27],q[110];
cx q[99],q[80];
cx q[14],q[100];
cx q[92],q[101];
cx q[11],q[121];
cx q[31],q[122];
cx q[32],q[123];
cx q[51],q[114];
cx q[29],q[124];
cx q[70],q[125];
cx q[72],q[126];
cx q[55],q[106];
x q[89];
cx q[52],q[117];
cx q[91],q[127];
cx q[94],q[118];
cx q[107],q[77];
cx q[97],q[128];
cx q[120],q[129];
cx q[93],q[130];
h q[132];
cx q[131],q[132];
tdg q[132];
cx q[0],q[132];
t q[132];
cx q[131],q[132];
tdg q[132];
cx q[0],q[132];
t q[131];
t q[132];
cx q[0],q[131];
h q[132];
t q[0];
tdg q[131];
cx q[0],q[131];
cx q[90],q[101];
cx q[35],q[133];
cx q[27],q[134];
cx q[92],q[135];
x q[106];
cx q[58],q[117];
cx q[75],q[127];
cx q[96],q[118];
cx q[79],q[128];
cx q[47],q[129];
cx q[94],q[130];
cx q[132],q[136];
cx q[90],q[137];
h q[138];
cx q[106],q[138];
tdg q[138];
cx q[89],q[138];
t q[138];
cx q[106],q[138];
tdg q[138];
cx q[89],q[138];
t q[106];
t q[138];
cx q[89],q[106];
h q[138];
t q[89];
tdg q[106];
cx q[89],q[106];
cx q[59],q[127];
x q[117];
cx q[118],q[139];
cx q[98],q[128];
cx q[110],q[129];
cx q[60],q[136];
x q[89];
x q[106];
x q[127];
cx q[77],q[139];
cx q[129],q[140];
cx q[61],q[136];
h q[141];
cx q[127],q[141];
tdg q[141];
cx q[117],q[141];
t q[141];
cx q[127],q[141];
tdg q[141];
cx q[117],q[141];
t q[127];
t q[141];
cx q[117],q[127];
h q[141];
t q[117];
tdg q[127];
cx q[117],q[127];
cx q[78],q[139];
cx q[80],q[140];
cx q[136],q[142];
x q[117];
x q[127];
h q[143];
cx q[141],q[143];
tdg q[143];
cx q[138],q[143];
t q[143];
cx q[141],q[143];
tdg q[143];
cx q[138],q[143];
t q[141];
t q[143];
cx q[138],q[141];
h q[143];
t q[138];
tdg q[141];
cx q[138],q[141];
h q[144];
cx q[128],q[144];
tdg q[144];
cx q[139],q[144];
t q[144];
cx q[128],q[144];
tdg q[144];
cx q[139],q[144];
t q[128];
t q[144];
cx q[139],q[128];
h q[144];
t q[139];
tdg q[128];
cx q[139],q[128];
cx q[130],q[140];
cx q[100],q[142];
cx q[101],q[142];
cx q[140],q[145];
cx q[139],q[146];
cx q[142],q[145];
cx q[128],q[146];
h q[147];
cx q[145],q[147];
tdg q[147];
cx q[144],q[147];
t q[147];
cx q[145],q[147];
tdg q[147];
cx q[144],q[147];
t q[145];
t q[147];
cx q[144],q[145];
h q[147];
t q[144];
tdg q[145];
cx q[144],q[145];
h q[148];
cx q[142],q[148];
tdg q[148];
cx q[140],q[148];
t q[148];
cx q[142],q[148];
tdg q[148];
cx q[140],q[148];
t q[142];
t q[148];
cx q[140],q[142];
h q[148];
t q[140];
tdg q[142];
cx q[140],q[142];
x q[147];
h q[149];
cx q[146],q[149];
tdg q[149];
cx q[148],q[149];
t q[149];
cx q[146],q[149];
tdg q[149];
cx q[148],q[149];
t q[146];
t q[149];
cx q[148],q[146];
h q[149];
t q[148];
tdg q[146];
cx q[148],q[146];
x q[149];
h q[150];
cx q[149],q[150];
tdg q[150];
cx q[147],q[150];
t q[150];
cx q[149],q[150];
tdg q[150];
cx q[147],q[150];
t q[149];
t q[150];
cx q[147],q[149];
h q[150];
t q[147];
tdg q[149];
cx q[147],q[149];
x q[150];
h q[151];
cx q[150],q[151];
tdg q[151];
cx q[143],q[151];
t q[151];
cx q[150],q[151];
tdg q[151];
cx q[143],q[151];
t q[150];
t q[151];
cx q[143],q[150];
h q[151];
t q[143];
tdg q[150];
cx q[143],q[150];
cx q[151],q[102];
h q[152];
cx q[128],q[152];
tdg q[152];
cx q[151],q[152];
t q[152];
cx q[128],q[152];
tdg q[152];
cx q[151],q[152];
t q[128];
t q[152];
cx q[151],q[128];
h q[152];
t q[151];
tdg q[128];
cx q[151],q[128];
cx q[151],q[81];
h q[153];
cx q[139],q[153];
tdg q[153];
cx q[151],q[153];
t q[153];
cx q[139],q[153];
tdg q[153];
cx q[151],q[153];
t q[139];
t q[153];
cx q[151],q[139];
h q[153];
t q[151];
tdg q[139];
cx q[151],q[139];
cx q[151],q[103];
h q[154];
cx q[142],q[154];
tdg q[154];
cx q[151],q[154];
t q[154];
cx q[142],q[154];
tdg q[154];
cx q[151],q[154];
t q[142];
t q[154];
cx q[151],q[142];
h q[154];
t q[151];
tdg q[142];
cx q[151],q[142];
cx q[151],q[82];
h q[155];
cx q[140],q[155];
tdg q[155];
cx q[151],q[155];
t q[155];
cx q[140],q[155];
tdg q[155];
cx q[151],q[155];
t q[140];
t q[155];
cx q[151],q[140];
h q[155];
t q[151];
tdg q[140];
cx q[151],q[140];
h q[151];
cx q[150],q[151];
tdg q[151];
cx q[143],q[151];
t q[151];
cx q[150],q[151];
tdg q[151];
cx q[143],q[151];
t q[150];
t q[151];
cx q[143],q[150];
h q[151];
t q[143];
tdg q[150];
cx q[143],q[150];
h q[151];
cx q[127],q[151];
tdg q[151];
cx q[117],q[151];
t q[151];
cx q[127],q[151];
tdg q[151];
cx q[117],q[151];
t q[127];
t q[151];
cx q[117],q[127];
h q[151];
t q[117];
tdg q[127];
cx q[117],q[127];
h q[102];
cx q[138],q[102];
tdg q[102];
cx q[151],q[102];
t q[102];
cx q[138],q[102];
tdg q[102];
cx q[151],q[102];
t q[138];
t q[102];
cx q[151],q[138];
h q[102];
t q[151];
tdg q[138];
cx q[151],q[138];
x q[127];
h q[81];
cx q[102],q[81];
tdg q[81];
cx q[150],q[81];
t q[81];
cx q[102],q[81];
tdg q[81];
cx q[150],q[81];
t q[102];
t q[81];
cx q[150],q[102];
h q[81];
t q[150];
tdg q[102];
cx q[150],q[102];
cx q[156],q[83];
h q[157];
cx q[128],q[157];
tdg q[157];
cx q[81],q[157];
t q[157];
cx q[128],q[157];
tdg q[157];
cx q[81],q[157];
t q[128];
t q[157];
cx q[81],q[128];
h q[157];
t q[81];
tdg q[128];
cx q[81],q[128];
cx q[156],q[64];
h q[158];
cx q[139],q[158];
tdg q[158];
cx q[81],q[158];
t q[158];
cx q[139],q[158];
tdg q[158];
cx q[81],q[158];
t q[139];
t q[158];
cx q[81],q[139];
h q[158];
t q[81];
tdg q[139];
cx q[81],q[139];
cx q[156],q[84];
h q[159];
cx q[142],q[159];
tdg q[159];
cx q[81],q[159];
t q[159];
cx q[142],q[159];
tdg q[159];
cx q[81],q[159];
t q[142];
t q[159];
cx q[81],q[142];
h q[159];
t q[81];
tdg q[142];
cx q[81],q[142];
cx q[156],q[65];
h q[160];
cx q[140],q[160];
tdg q[160];
cx q[81],q[160];
t q[160];
cx q[140],q[160];
tdg q[160];
cx q[81],q[160];
t q[140];
t q[160];
cx q[81],q[140];
h q[160];
t q[81];
tdg q[140];
cx q[81],q[140];
h q[81];
cx q[102],q[81];
tdg q[81];
cx q[150],q[81];
t q[81];
cx q[102],q[81];
tdg q[81];
cx q[150],q[81];
t q[102];
t q[81];
cx q[150],q[102];
h q[81];
t q[150];
tdg q[102];
cx q[150],q[102];
h q[102];
cx q[138],q[102];
tdg q[102];
cx q[151],q[102];
t q[102];
cx q[138],q[102];
tdg q[102];
cx q[151],q[102];
t q[138];
t q[102];
cx q[151],q[138];
h q[102];
t q[151];
tdg q[138];
cx q[151],q[138];
h q[102];
cx q[106],q[102];
tdg q[102];
cx q[89],q[102];
t q[102];
cx q[106],q[102];
tdg q[102];
cx q[89],q[102];
t q[106];
t q[102];
cx q[89],q[106];
h q[102];
t q[89];
tdg q[106];
cx q[89],q[106];
h q[81];
cx q[141],q[81];
tdg q[81];
cx q[102],q[81];
t q[81];
cx q[141],q[81];
tdg q[81];
cx q[102],q[81];
t q[141];
t q[81];
cx q[102],q[141];
h q[81];
t q[102];
tdg q[141];
cx q[102],q[141];
x q[89];
h q[103];
cx q[150],q[103];
tdg q[103];
cx q[81],q[103];
t q[103];
cx q[150],q[103];
tdg q[103];
cx q[81],q[103];
t q[150];
t q[103];
cx q[81],q[150];
h q[103];
t q[81];
tdg q[150];
cx q[81],q[150];
h q[143];
cx q[141],q[143];
tdg q[143];
cx q[138],q[143];
t q[143];
cx q[141],q[143];
tdg q[143];
cx q[138],q[143];
t q[141];
t q[143];
cx q[138],q[141];
h q[143];
t q[138];
tdg q[141];
cx q[138],q[141];
cx q[161],q[133];
h q[162];
cx q[128],q[162];
tdg q[162];
cx q[103],q[162];
t q[162];
cx q[128],q[162];
tdg q[162];
cx q[103],q[162];
t q[128];
t q[162];
cx q[103],q[128];
h q[162];
t q[103];
tdg q[128];
cx q[103],q[128];
cx q[161],q[121];
h q[163];
cx q[139],q[163];
tdg q[163];
cx q[103],q[163];
t q[163];
cx q[139],q[163];
tdg q[163];
cx q[103],q[163];
t q[139];
t q[163];
cx q[103],q[139];
h q[163];
t q[103];
tdg q[139];
cx q[103],q[139];
cx q[161],q[50];
h q[164];
cx q[142],q[164];
tdg q[164];
cx q[103],q[164];
t q[164];
cx q[142],q[164];
tdg q[164];
cx q[103],q[164];
t q[142];
t q[164];
cx q[103],q[142];
h q[164];
t q[103];
tdg q[142];
cx q[103],q[142];
cx q[161],q[111];
h q[165];
cx q[140],q[165];
tdg q[165];
cx q[103],q[165];
t q[165];
cx q[140],q[165];
tdg q[165];
cx q[103],q[165];
t q[140];
t q[165];
cx q[103],q[140];
h q[165];
t q[103];
tdg q[140];
cx q[103],q[140];
h q[103];
cx q[150],q[103];
tdg q[103];
cx q[81],q[103];
t q[103];
cx q[150],q[103];
tdg q[103];
cx q[81],q[103];
t q[150];
t q[103];
cx q[81],q[150];
h q[103];
t q[81];
tdg q[150];
cx q[81],q[150];
h q[103];
cx q[151],q[103];
tdg q[103];
cx q[102],q[103];
t q[103];
cx q[151],q[103];
tdg q[103];
cx q[102],q[103];
t q[151];
t q[103];
cx q[102],q[151];
h q[103];
t q[102];
tdg q[151];
cx q[102],q[151];
h q[82];
cx q[103],q[82];
tdg q[82];
cx q[150],q[82];
t q[82];
cx q[103],q[82];
tdg q[82];
cx q[150],q[82];
t q[103];
t q[82];
cx q[150],q[103];
h q[82];
t q[150];
tdg q[103];
cx q[150],q[103];
cx q[166],q[122];
h q[167];
cx q[128],q[167];
tdg q[167];
cx q[82],q[167];
t q[167];
cx q[128],q[167];
tdg q[167];
cx q[82],q[167];
t q[128];
t q[167];
cx q[82],q[128];
h q[167];
t q[82];
tdg q[128];
cx q[82],q[128];
cx q[166],q[112];
h q[168];
cx q[139],q[168];
tdg q[168];
cx q[82],q[168];
t q[168];
cx q[139],q[168];
tdg q[168];
cx q[82],q[168];
t q[139];
t q[168];
cx q[82],q[139];
h q[168];
t q[82];
tdg q[139];
cx q[82],q[139];
cx q[166],q[123];
h q[169];
cx q[142],q[169];
tdg q[169];
cx q[82],q[169];
t q[169];
cx q[142],q[169];
tdg q[169];
cx q[82],q[169];
t q[142];
t q[169];
cx q[82],q[142];
h q[169];
t q[82];
tdg q[142];
cx q[82],q[142];
cx q[166],q[113];
x q[142];
h q[170];
cx q[140],q[170];
tdg q[170];
cx q[82],q[170];
t q[170];
cx q[140],q[170];
tdg q[170];
cx q[82],q[170];
t q[140];
t q[170];
cx q[82],q[140];
h q[170];
t q[82];
tdg q[140];
cx q[82],q[140];
h q[82];
cx q[103],q[82];
tdg q[82];
cx q[150],q[82];
t q[82];
cx q[103],q[82];
tdg q[82];
cx q[150],q[82];
t q[103];
t q[82];
cx q[150],q[103];
h q[82];
t q[150];
tdg q[103];
cx q[150],q[103];
x q[140];
h q[103];
cx q[151],q[103];
tdg q[103];
cx q[102],q[103];
t q[103];
cx q[151],q[103];
tdg q[103];
cx q[102],q[103];
t q[151];
t q[103];
cx q[102],q[151];
h q[103];
t q[102];
tdg q[151];
cx q[102],q[151];
x q[150];
cx q[12],q[166];
x q[140];
h q[150];
cx q[149],q[150];
tdg q[150];
cx q[147],q[150];
t q[150];
cx q[149],q[150];
tdg q[150];
cx q[147],q[150];
t q[149];
t q[150];
cx q[147],q[149];
h q[150];
t q[147];
tdg q[149];
cx q[147],q[149];
cx q[69],q[166];
h q[121];
cx q[142],q[121];
tdg q[121];
cx q[140],q[121];
t q[121];
cx q[142],q[121];
tdg q[121];
cx q[140],q[121];
t q[142];
t q[121];
cx q[140],q[142];
h q[121];
t q[140];
tdg q[142];
cx q[140],q[142];
h q[81];
cx q[141],q[81];
tdg q[81];
cx q[102],q[81];
t q[81];
cx q[141],q[81];
tdg q[81];
cx q[102],q[81];
t q[141];
t q[81];
cx q[102],q[141];
h q[81];
t q[102];
tdg q[141];
cx q[102],q[141];
x q[149];
x q[147];
cx q[91],q[150];
cx q[13],q[166];
x q[140];
x q[142];
h q[149];
cx q[146],q[149];
tdg q[149];
cx q[148],q[149];
t q[149];
cx q[146],q[149];
tdg q[149];
cx q[148],q[149];
t q[146];
t q[149];
cx q[148],q[146];
h q[149];
t q[148];
tdg q[146];
cx q[148],q[146];
h q[147];
cx q[145],q[147];
tdg q[147];
cx q[144],q[147];
t q[147];
cx q[145],q[147];
tdg q[147];
cx q[144],q[147];
t q[145];
t q[147];
cx q[144],q[145];
h q[147];
t q[144];
tdg q[145];
cx q[144],q[145];
cx q[166],q[114];
cx q[139],q[146];
cx q[32],q[149];
cx q[4],q[147];
cx q[114],q[171];
cx q[128],q[146];
x q[149];
cx q[5],q[147];
cx q[106],q[171];
h q[144];
cx q[128],q[144];
tdg q[144];
cx q[139],q[144];
t q[144];
cx q[128],q[144];
tdg q[144];
cx q[139],q[144];
t q[128];
t q[144];
cx q[139],q[128];
h q[144];
t q[139];
tdg q[128];
cx q[139],q[128];
cx q[149],q[147];
h q[64];
cx q[106],q[64];
tdg q[64];
cx q[172],q[64];
t q[64];
cx q[106],q[64];
tdg q[64];
cx q[172],q[64];
t q[106];
t q[64];
cx q[172],q[106];
h q[64];
t q[172];
tdg q[106];
cx q[172],q[106];
cx q[118],q[144];
x q[128];
cx q[147],q[150];
cx q[101],q[144];
cx q[59],q[150];
cx q[118],q[139];
cx q[78],q[144];
h q[103];
cx q[150],q[103];
tdg q[103];
cx q[117],q[103];
t q[103];
cx q[150],q[103];
tdg q[103];
cx q[117],q[103];
t q[150];
t q[103];
cx q[117],q[150];
h q[103];
t q[117];
tdg q[150];
cx q[117],q[150];
cx q[77],q[139];
x q[144];
h q[83];
cx q[156],q[83];
tdg q[83];
cx q[103],q[83];
t q[83];
cx q[156],q[83];
tdg q[83];
cx q[103],q[83];
t q[156];
t q[83];
cx q[103],q[156];
h q[83];
t q[103];
tdg q[156];
cx q[103],q[156];
cx q[117],q[173];
cx q[78],q[139];
cx q[45],q[77];
h q[146];
cx q[128],q[146];
tdg q[146];
cx q[144],q[146];
t q[146];
cx q[128],q[146];
tdg q[146];
cx q[144],q[146];
t q[128];
t q[146];
cx q[144],q[128];
h q[146];
t q[144];
tdg q[128];
cx q[144],q[128];
x q[174];
cx q[150],q[173];
x q[144];
x q[128];
h q[65];
cx q[84],q[65];
tdg q[65];
cx q[64],q[65];
t q[65];
cx q[84],q[65];
tdg q[65];
cx q[64],q[65];
t q[84];
t q[65];
cx q[64],q[84];
h q[65];
t q[64];
tdg q[84];
cx q[64],q[84];
x q[175];
x q[144];
x q[128];
h q[161];
cx q[65],q[161];
tdg q[161];
cx q[83],q[161];
t q[161];
cx q[65],q[161];
tdg q[161];
cx q[83],q[161];
t q[65];
t q[161];
cx q[83],q[65];
h q[161];
t q[83];
tdg q[65];
cx q[83],q[65];
x q[176];
h q[133];
cx q[161],q[133];
tdg q[133];
cx q[146],q[133];
t q[133];
cx q[161],q[133];
tdg q[133];
cx q[146],q[133];
t q[161];
t q[133];
cx q[146],q[161];
h q[133];
t q[146];
tdg q[161];
cx q[146],q[161];
h q[50];
cx q[121],q[50];
tdg q[50];
cx q[133],q[50];
t q[50];
cx q[121],q[50];
tdg q[50];
cx q[133],q[50];
t q[121];
t q[50];
cx q[133],q[121];
h q[50];
t q[133];
tdg q[121];
cx q[133],q[121];
cx q[177],q[104];
h q[178];
cx q[106],q[178];
tdg q[178];
cx q[50],q[178];
t q[178];
cx q[106],q[178];
tdg q[178];
cx q[50],q[178];
t q[106];
t q[178];
cx q[50],q[106];
h q[178];
t q[50];
tdg q[106];
cx q[50],q[106];
cx q[177],q[85];
h q[179];
cx q[172],q[179];
tdg q[179];
cx q[50],q[179];
t q[179];
cx q[172],q[179];
tdg q[179];
cx q[50],q[179];
t q[172];
t q[179];
cx q[50],q[172];
h q[179];
t q[50];
tdg q[172];
cx q[50],q[172];
cx q[177],q[105];
h q[180];
cx q[117],q[180];
tdg q[180];
cx q[50],q[180];
t q[180];
cx q[117],q[180];
tdg q[180];
cx q[50],q[180];
t q[117];
t q[180];
cx q[50],q[117];
h q[180];
t q[50];
tdg q[117];
cx q[50],q[117];
cx q[177],q[86];
h q[181];
cx q[150],q[181];
tdg q[181];
cx q[50],q[181];
t q[181];
cx q[150],q[181];
tdg q[181];
cx q[50],q[181];
t q[150];
t q[181];
cx q[50],q[150];
h q[181];
t q[50];
tdg q[150];
cx q[50],q[150];
h q[50];
cx q[121],q[50];
tdg q[50];
cx q[133],q[50];
t q[50];
cx q[121],q[50];
tdg q[50];
cx q[133],q[50];
t q[121];
t q[50];
cx q[133],q[121];
h q[50];
t q[133];
tdg q[121];
cx q[133],q[121];
h q[50];
cx q[142],q[50];
tdg q[50];
cx q[140],q[50];
t q[50];
cx q[142],q[50];
tdg q[50];
cx q[140],q[50];
t q[142];
t q[50];
cx q[140],q[142];
h q[50];
t q[140];
tdg q[142];
cx q[140],q[142];
h q[111];
cx q[50],q[111];
tdg q[111];
cx q[133],q[111];
t q[111];
cx q[50],q[111];
tdg q[111];
cx q[133],q[111];
t q[50];
t q[111];
cx q[133],q[50];
h q[111];
t q[133];
tdg q[50];
cx q[133],q[50];
h q[148];
cx q[142],q[148];
tdg q[148];
cx q[140],q[148];
t q[148];
cx q[142],q[148];
tdg q[148];
cx q[140],q[148];
t q[142];
t q[148];
cx q[140],q[142];
h q[148];
t q[140];
tdg q[142];
cx q[140],q[142];
cx q[182],q[87];
cx q[140],q[145];
h q[183];
cx q[106],q[183];
tdg q[183];
cx q[111],q[183];
t q[183];
cx q[106],q[183];
tdg q[183];
cx q[111],q[183];
t q[106];
t q[183];
cx q[111],q[106];
h q[183];
t q[111];
tdg q[106];
cx q[111],q[106];
cx q[142],q[145];
x q[140];
cx q[182],q[66];
x q[142];
h q[184];
cx q[172],q[184];
tdg q[184];
cx q[111],q[184];
t q[184];
cx q[172],q[184];
tdg q[184];
cx q[111],q[184];
t q[172];
t q[184];
cx q[111],q[172];
h q[184];
t q[111];
tdg q[172];
cx q[111],q[172];
cx q[182],q[134];
h q[185];
cx q[117],q[185];
tdg q[185];
cx q[111],q[185];
t q[185];
cx q[117],q[185];
tdg q[185];
cx q[111],q[185];
t q[117];
t q[185];
cx q[111],q[117];
h q[185];
t q[111];
tdg q[117];
cx q[111],q[117];
cx q[182],q[124];
h q[186];
cx q[150],q[186];
tdg q[186];
cx q[111],q[186];
t q[186];
cx q[150],q[186];
tdg q[186];
cx q[111],q[186];
t q[150];
t q[186];
cx q[111],q[150];
h q[186];
t q[111];
tdg q[150];
cx q[111],q[150];
h q[111];
cx q[50],q[111];
tdg q[111];
cx q[133],q[111];
t q[111];
cx q[50],q[111];
tdg q[111];
cx q[133],q[111];
t q[50];
t q[111];
cx q[133],q[50];
h q[111];
t q[133];
tdg q[50];
cx q[133],q[50];
h q[133];
cx q[161],q[133];
tdg q[133];
cx q[146],q[133];
t q[133];
cx q[161],q[133];
tdg q[133];
cx q[146],q[133];
t q[161];
t q[133];
cx q[146],q[161];
h q[133];
t q[146];
tdg q[161];
cx q[146],q[161];
x q[176];
h q[146];
cx q[128],q[146];
tdg q[146];
cx q[144],q[146];
t q[146];
cx q[128],q[146];
tdg q[146];
cx q[144],q[146];
t q[128];
t q[146];
cx q[144],q[128];
h q[146];
t q[144];
tdg q[128];
cx q[144],q[128];
h q[161];
cx q[65],q[161];
tdg q[161];
cx q[83],q[161];
t q[161];
cx q[65],q[161];
tdg q[161];
cx q[83],q[161];
t q[65];
t q[161];
cx q[83],q[65];
h q[161];
t q[83];
tdg q[65];
cx q[83],q[65];
x q[144];
x q[128];
h q[161];
cx q[128],q[161];
tdg q[161];
cx q[144],q[161];
t q[161];
cx q[128],q[161];
tdg q[161];
cx q[144],q[161];
t q[128];
t q[161];
cx q[144],q[128];
h q[161];
t q[144];
tdg q[128];
cx q[144],q[128];
h q[146];
cx q[65],q[146];
tdg q[146];
cx q[83],q[146];
t q[146];
cx q[65],q[146];
tdg q[146];
cx q[83],q[146];
t q[65];
t q[146];
cx q[83],q[65];
h q[146];
t q[83];
tdg q[65];
cx q[83],q[65];
x q[146];
h q[133];
cx q[146],q[133];
tdg q[133];
cx q[161],q[133];
t q[133];
cx q[146],q[133];
tdg q[133];
cx q[161],q[133];
t q[146];
t q[133];
cx q[161],q[146];
h q[133];
t q[161];
tdg q[146];
cx q[161],q[146];
h q[111];
cx q[121],q[111];
tdg q[111];
cx q[133],q[111];
t q[111];
cx q[121],q[111];
tdg q[111];
cx q[133],q[111];
t q[121];
t q[111];
cx q[133],q[121];
h q[111];
t q[133];
tdg q[121];
cx q[133],q[121];
cx q[182],q[88];
h q[187];
cx q[106],q[187];
tdg q[187];
cx q[111],q[187];
t q[187];
cx q[106],q[187];
tdg q[187];
cx q[111],q[187];
t q[106];
t q[187];
cx q[111],q[106];
h q[187];
t q[111];
tdg q[106];
cx q[111],q[106];
cx q[182],q[67];
h q[188];
cx q[172],q[188];
tdg q[188];
cx q[111],q[188];
t q[188];
cx q[172],q[188];
tdg q[188];
cx q[111],q[188];
t q[172];
t q[188];
cx q[111],q[172];
h q[188];
t q[111];
tdg q[172];
cx q[111],q[172];
cx q[182],q[137];
h q[189];
cx q[117],q[189];
tdg q[189];
cx q[111],q[189];
t q[189];
cx q[117],q[189];
tdg q[189];
cx q[111],q[189];
t q[117];
t q[189];
cx q[111],q[117];
h q[189];
t q[111];
tdg q[117];
cx q[111],q[117];
cx q[182],q[135];
h q[190];
cx q[150],q[190];
tdg q[190];
cx q[111],q[190];
t q[190];
cx q[150],q[190];
tdg q[190];
cx q[111],q[190];
t q[150];
t q[190];
cx q[111],q[150];
h q[190];
t q[111];
tdg q[150];
cx q[111],q[150];
h q[111];
cx q[121],q[111];
tdg q[111];
cx q[133],q[111];
t q[111];
cx q[121],q[111];
tdg q[111];
cx q[133],q[111];
t q[121];
t q[111];
cx q[133],q[121];
h q[111];
t q[133];
tdg q[121];
cx q[133],q[121];
h q[111];
cx q[50],q[111];
tdg q[111];
cx q[133],q[111];
t q[111];
cx q[50],q[111];
tdg q[111];
cx q[133],q[111];
t q[50];
t q[111];
cx q[133],q[50];
h q[111];
t q[133];
tdg q[50];
cx q[133],q[50];
h q[121];
cx q[142],q[121];
tdg q[121];
cx q[140],q[121];
t q[121];
cx q[142],q[121];
tdg q[121];
cx q[140],q[121];
t q[142];
t q[121];
cx q[140],q[142];
h q[121];
t q[140];
tdg q[142];
cx q[140],q[142];
cx q[182],q[125];
x q[140];
x q[142];
h q[191];
cx q[106],q[191];
tdg q[191];
cx q[111],q[191];
t q[191];
cx q[106],q[191];
tdg q[191];
cx q[111],q[191];
t q[106];
t q[191];
cx q[111],q[106];
h q[191];
t q[111];
tdg q[106];
cx q[111],q[106];
cx q[182],q[115];
x q[106];
h q[192];
cx q[172],q[192];
tdg q[192];
cx q[111],q[192];
t q[192];
cx q[172],q[192];
tdg q[192];
cx q[111],q[192];
t q[172];
t q[192];
cx q[111],q[172];
h q[192];
t q[111];
tdg q[172];
cx q[111],q[172];
h q[138];
cx q[106],q[138];
tdg q[138];
cx q[89],q[138];
t q[138];
cx q[106],q[138];
tdg q[138];
cx q[89],q[138];
t q[106];
t q[138];
cx q[89],q[106];
h q[138];
t q[89];
tdg q[106];
cx q[89],q[106];
cx q[182],q[126];
x q[89];
x q[106];
h q[193];
cx q[117],q[193];
tdg q[193];
cx q[111],q[193];
t q[193];
cx q[117],q[193];
tdg q[193];
cx q[111],q[193];
t q[117];
t q[193];
cx q[111],q[117];
h q[193];
t q[111];
tdg q[117];
cx q[111],q[117];
h q[102];
cx q[106],q[102];
tdg q[102];
cx q[89],q[102];
t q[102];
cx q[106],q[102];
tdg q[102];
cx q[89],q[102];
t q[106];
t q[102];
cx q[89],q[106];
h q[102];
t q[89];
tdg q[106];
cx q[89],q[106];
cx q[182],q[116];
x q[117];
cx q[33],q[89];
h q[194];
cx q[150],q[194];
tdg q[194];
cx q[111],q[194];
t q[194];
cx q[150],q[194];
tdg q[194];
cx q[111],q[194];
t q[150];
t q[194];
cx q[111],q[150];
h q[194];
t q[111];
tdg q[150];
cx q[111],q[150];
h q[141];
cx q[127],q[141];
tdg q[141];
cx q[117],q[141];
t q[141];
cx q[127],q[141];
tdg q[141];
cx q[117],q[141];
t q[127];
t q[141];
cx q[117],q[127];
h q[141];
t q[117];
tdg q[127];
cx q[117],q[127];
cx q[51],q[89];
h q[111];
cx q[50],q[111];
tdg q[111];
cx q[133],q[111];
t q[111];
cx q[50],q[111];
tdg q[111];
cx q[133],q[111];
t q[50];
t q[111];
cx q[133],q[50];
h q[111];
t q[133];
tdg q[50];
cx q[133],q[50];
x q[117];
x q[127];
h q[133];
cx q[146],q[133];
tdg q[133];
cx q[161],q[133];
t q[133];
cx q[146],q[133];
tdg q[133];
cx q[161],q[133];
t q[146];
t q[133];
cx q[161],q[146];
h q[133];
t q[161];
tdg q[146];
cx q[161],q[146];
h q[151];
cx q[127],q[151];
tdg q[151];
cx q[117],q[151];
t q[151];
cx q[127],q[151];
tdg q[151];
cx q[117],q[151];
t q[127];
t q[151];
cx q[117],q[127];
h q[151];
t q[117];
tdg q[127];
cx q[117],q[127];
h q[50];
cx q[142],q[50];
tdg q[50];
cx q[140],q[50];
t q[50];
cx q[142],q[50];
tdg q[50];
cx q[140],q[50];
t q[142];
t q[50];
cx q[140],q[142];
h q[50];
t q[140];
tdg q[142];
cx q[140],q[142];
x q[146];
h q[161];
cx q[128],q[161];
tdg q[161];
cx q[144],q[161];
t q[161];
cx q[128],q[161];
tdg q[161];
cx q[144],q[161];
t q[128];
t q[161];
cx q[144],q[128];
h q[161];
t q[144];
tdg q[128];
cx q[144],q[128];
cx q[136],q[142];
x q[140];
cx q[91],q[127];
h q[146];
cx q[65],q[146];
tdg q[146];
cx q[83],q[146];
t q[146];
cx q[65],q[146];
tdg q[146];
cx q[83],q[146];
t q[65];
t q[146];
cx q[83],q[65];
h q[146];
t q[83];
tdg q[65];
cx q[83],q[65];
cx q[97],q[128];
cx q[118],q[144];
cx q[100],q[142];
cx q[129],q[140];
cx q[75],q[127];
cx q[132],q[136];
cx q[79],q[128];
x q[175];
x q[174];
cx q[101],q[144];
cx q[63],q[100];
cx q[80],q[140];
cx q[59],q[127];
cx q[4],q[75];
cx q[60],q[97];
cx q[120],q[129];
cx q[93],q[118];
cx q[98],q[128];
h q[65];
cx q[84],q[65];
tdg q[65];
cx q[64],q[65];
t q[65];
cx q[84],q[65];
tdg q[65];
cx q[64],q[65];
t q[84];
t q[65];
cx q[64],q[84];
h q[65];
t q[64];
tdg q[84];
cx q[64],q[84];
h q[83];
cx q[156],q[83];
tdg q[83];
cx q[103],q[83];
t q[83];
cx q[156],q[83];
tdg q[83];
cx q[103],q[83];
t q[156];
t q[83];
cx q[103],q[156];
h q[83];
t q[103];
tdg q[156];
cx q[103],q[156];
cx q[78],q[144];
cx q[101],q[142];
cx q[32],q[100];
cx q[39],q[63];
cx q[130],q[140];
cx q[49],q[80];
cx q[6],q[75];
cx q[61],q[97];
cx q[60],q[136];
cx q[47],q[79];
cx q[94],q[118];
cx q[62],q[98];
cx q[117],q[173];
h q[64];
cx q[106],q[64];
tdg q[64];
cx q[172],q[64];
t q[64];
cx q[106],q[64];
tdg q[64];
cx q[172],q[64];
t q[106];
t q[64];
cx q[172],q[106];
h q[64];
t q[172];
tdg q[106];
cx q[172],q[106];
cx q[46],q[78];
cx q[14],q[100];
cx q[42],q[63];
cx q[5],q[80];
cx q[17],q[49];
cx q[109],q[97];
cx q[61],q[136];
cx q[34],q[60];
cx q[47],q[129];
cx q[45],q[101];
cx q[96],q[118];
cx q[93],q[130];
cx q[31],q[98];
cx q[38],q[62];
cx q[150],q[173];
cx q[114],q[171];
cx q[3],q[78];
cx q[15],q[46];
cx q[99],q[80];
cx q[40],q[60];
cx q[44],q[61];
cx q[110],q[129];
cx q[9],q[45];
cx q[94],q[130];
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
cx q[35],q[98];
cx q[41],q[62];
h q[103];
cx q[150],q[103];
tdg q[103];
cx q[117],q[103];
t q[103];
cx q[150],q[103];
tdg q[103];
cx q[117],q[103];
t q[150];
t q[103];
cx q[117],q[150];
h q[103];
t q[117];
tdg q[150];
cx q[117],q[150];
cx q[106],q[171];
cx q[11],q[78];
cx q[33],q[114];
x q[99];
cx q[43],q[61];
cx q[21],q[45];
h q[96];
cx q[95],q[96];
tdg q[96];
cx q[0],q[96];
t q[96];
cx q[95],q[96];
tdg q[96];
cx q[0],q[96];
t q[95];
t q[96];
cx q[0],q[95];
h q[96];
t q[0];
tdg q[95];
cx q[0],q[95];
cx q[71],q[117];
cx q[53],q[106];
cx q[91],q[150];
cx q[51],q[114];
cx q[2],q[33];
h q[132];
cx q[131],q[132];
tdg q[132];
cx q[0],q[132];
t q[132];
cx q[131],q[132];
tdg q[132];
cx q[0],q[132];
t q[131];
t q[132];
cx q[0],q[131];
h q[132];
t q[0];
tdg q[131];
cx q[0],q[131];
cx q[52],q[117];
cx q[24],q[71];
cx q[54],q[106];
cx q[147],q[150];
cx q[16],q[91];
cx q[37],q[53];
cx q[166],q[114];
cx q[4],q[33];
cx q[8],q[51];
h q[120];
cx q[119],q[120];
tdg q[120];
cx q[0],q[120];
t q[120];
cx q[119],q[120];
tdg q[120];
cx q[0],q[120];
t q[119];
t q[120];
cx q[0],q[119];
h q[120];
t q[0];
tdg q[119];
cx q[0],q[119];
cx q[58],q[117];
cx q[26],q[71];
cx q[55],q[106];
cx q[59],q[150];
cx q[18],q[91];
cx q[16],q[53];
cx q[52],q[89];
cx q[6],q[33];
cx q[4],q[147];
cx q[20],q[54];
cx q[68],q[51];
cx q[7],q[8];
h q[37];
cx q[36],q[37];
tdg q[37];
cx q[0],q[37];
t q[37];
cx q[36],q[37];
tdg q[37];
cx q[0],q[37];
t q[36];
t q[37];
cx q[0],q[36];
h q[37];
t q[0];
tdg q[36];
cx q[0],q[36];
cx q[57],q[71];
cx q[24],q[55];
cx q[74],q[91];
cx q[18],q[53];
cx q[15],q[16];
cx q[12],q[52];
cx q[5],q[147];
cx q[3],q[4];
cx q[70],q[54];
cx q[19],q[20];
cx q[10],q[51];
cx q[34],q[8];
cx q[28],q[58];
cx q[30],q[59];
cx q[7],q[47];
cx q[68],q[93];
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
cx q[26],q[55];
cx q[23],q[24];
cx q[38],q[16];
cx q[17],q[18];
cx q[69],q[52];
cx q[12],q[166];
cx q[149],q[147];
cx q[31],q[4];
cx q[5],q[6];
cx q[22],q[54];
cx q[40],q[20];
cx q[72],q[58];
cx q[27],q[28];
cx q[76],q[59];
cx q[29],q[30];
cx q[19],q[47];
x q[10];
cx q[70],q[93];
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
cx q[41],q[24];
cx q[23],q[46];
cx q[25],q[26];
cx q[39],q[18];
cx q[14],q[52];
cx q[69],q[166];
cx q[11],q[12];
cx q[32],q[6];
x q[149];
cx q[90],q[58];
cx q[43],q[28];
cx q[92],q[59];
cx q[44],q[30];
cx q[29],q[79];
x q[22];
cx q[9],q[10];
cx q[76],q[94];
h q[74];
cx q[73],q[74];
tdg q[74];
cx q[0],q[74];
t q[74];
cx q[73],q[74];
tdg q[74];
cx q[0],q[74];
t q[73];
t q[74];
cx q[0],q[73];
h q[74];
t q[0];
tdg q[73];
cx q[0],q[73];
cx q[42],q[26];
cx q[25],q[49];
cx q[13],q[166];
cx q[35],q[12];
x q[14];
cx q[69],q[99];
cx q[32],q[149];
cx q[48],q[79];
cx q[29],q[110];
cx q[92],q[77];
cx q[21],q[22];
cx q[72],q[94];
cx q[13],q[14];
x q[48];
cx q[107],q[77];
cx q[92],q[101];
cx q[27],q[48];
cx q[90],q[101];
x q[107];
cx q[27],q[110];
cx q[90],q[107];
