xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 36;
 0.08326;-2.46263;0.51114;,
 -0.08771;-1.31132;2.15685;,
 -3.41586;-2.19879;2.57245;,
 -3.36988;-3.41818;0.50015;,
 0.04666;-1.09175;-0.76308;,
 -3.42914;-2.12746;-1.17089;,
 0.12985;1.10735;-0.75931;,
 -3.38052;1.91353;-1.17561;,
 0.16556;2.13792;0.50848;,
 -3.38732;2.99192;0.49717;,
 0.04977;1.03195;2.13597;,
 -3.40644;1.91563;2.57822;,
 -15.87763;-0.59844;1.87257;,
 -15.86998;-1.47728;0.47870;,
 -15.82635;-0.57865;-0.75072;,
 -15.82131;1.31497;-0.75926;,
 -15.83688;1.89336;0.47644;,
 -15.88661;1.31862;1.87445;,
 1.40862;0.29462;0.16036;,
 1.42188;0.67742;0.63128;,
 1.37771;-0.52223;0.15896;,
 1.39131;-1.03144;0.63227;,
 1.37887;0.26661;1.23581;,
 1.32781;-0.60378;1.24356;,
 0.52163;-1.01152;-0.62432;,
 0.55479;-2.25354;0.53014;,
 0.59700;0.98089;-0.62091;,
 0.62936;1.91457;0.52773;,
 0.52445;0.91256;2.00224;,
 0.39990;-1.21045;2.02116;,
 0.89476;-0.88413;-0.41732;,
 0.92276;-1.93293;0.55753;,
 0.95841;0.79832;-0.41444;,
 0.98573;1.58675;0.55550;,
 0.89714;0.74063;1.80062;,
 0.79197;-1.05212;1.81660;;
 
 38;
 4;0,1,2,3;,
 4;4,0,3,5;,
 4;6,4,5,7;,
 4;8,6,7,9;,
 4;10,8,9,11;,
 4;1,10,11,2;,
 4;3,2,12,13;,
 4;5,3,13,14;,
 4;7,5,14,15;,
 4;9,7,15,16;,
 4;11,9,16,17;,
 4;2,11,17,12;,
 3;18,19,20;,
 3;20,19,21;,
 3;19,22,21;,
 3;22,23,21;,
 4;0,4,24,25;,
 4;4,6,26,24;,
 4;6,8,27,26;,
 4;8,10,28,27;,
 4;10,1,29,28;,
 4;1,0,25,29;,
 4;25,24,30,31;,
 4;24,26,32,30;,
 4;26,27,33,32;,
 4;27,28,34,33;,
 4;28,29,35,34;,
 4;29,25,31,35;,
 4;31,30,20,21;,
 4;30,32,18,20;,
 4;32,33,19,18;,
 4;33,34,22,19;,
 4;34,35,23,22;,
 4;35,31,21,23;,
 3;17,16,12;,
 3;12,16,13;,
 3;16,15,13;,
 3;15,14,13;;
 
 MeshMaterialList {
  1;
  38;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  53;
  0.270120;-0.441290;0.855744;,
  0.284791;-0.784721;0.550552;,
  0.258642;-0.368589;-0.892886;,
  0.275565;0.414866;-0.867151;,
  0.297074;0.754491;-0.585228;,
  0.277531;0.448854;0.849416;,
  0.054280;-0.477449;0.876981;,
  0.060435;-0.842913;0.534645;,
  0.058478;-0.421338;-0.905016;,
  0.069247;0.475942;-0.876746;,
  0.032121;0.969824;-0.241680;,
  0.069932;0.510352;0.857117;,
  -0.109897;-0.486152;0.866937;,
  -0.135501;-0.846938;0.514135;,
  -0.086117;-0.444785;-0.891488;,
  -0.033601;-0.002587;-0.999432;,
  -0.077182;0.996342;-0.036685;,
  -0.072269;0.905651;0.417819;,
  0.429469;-0.608656;-0.667154;,
  0.432160;-0.328420;-0.839868;,
  0.476845;0.355974;-0.803680;,
  0.490167;0.669552;-0.558065;,
  0.458872;0.388079;0.799269;,
  0.437612;-0.399021;0.805778;,
  0.811366;-0.528016;-0.250769;,
  0.688843;-0.262454;-0.675732;,
  0.731873;0.255082;-0.631898;,
  0.835488;0.518502;-0.181979;,
  0.708798;0.279622;0.647624;,
  0.682986;-0.312859;0.660037;,
  0.979989;-0.197609;0.023931;,
  0.947476;-0.124119;-0.294760;,
  0.932905;0.112727;-0.342025;,
  0.987776;0.155774;0.005787;,
  0.942903;0.083710;0.322378;,
  0.886398;-0.190145;0.422070;,
  0.280191;-0.678184;-0.679381;,
  0.064239;-0.767018;-0.638402;,
  0.293563;0.798814;0.525087;,
  0.240297;0.829712;0.503821;,
  -0.120552;-0.793601;-0.596377;,
  -0.065513;0.872995;-0.483309;,
  -0.056237;-0.001255;0.998417;,
  0.461218;0.718275;0.520920;,
  0.430337;-0.718036;0.547024;,
  0.569451;0.660497;0.489356;,
  0.538892;-0.662921;0.519742;,
  -0.999287;-0.004645;-0.037457;,
  -0.999743;0.004991;-0.022108;,
  -0.999695;0.002583;-0.024569;,
  -0.999756;0.007375;-0.020831;,
  -0.999658;0.006157;-0.025409;,
  -0.999431;0.002508;-0.033632;;
  38;
  4;1,0,6,7;,
  4;2,36,37,8;,
  4;3,2,8,9;,
  4;4,3,9,10;,
  4;5,38,39,11;,
  4;0,5,11,6;,
  4;7,6,12,13;,
  4;8,37,40,14;,
  4;9,8,14,15;,
  4;10,9,41,16;,
  4;11,10,16,17;,
  4;6,11,42,12;,
  3;32,33,31;,
  3;31,33,30;,
  3;33,34,30;,
  3;34,35,30;,
  4;36,2,19,18;,
  4;2,3,20,19;,
  4;3,4,21,20;,
  4;38,5,22,43;,
  4;5,0,23,22;,
  4;0,1,44,23;,
  4;18,19,25,24;,
  4;19,20,26,25;,
  4;20,21,27,26;,
  4;43,22,28,45;,
  4;22,23,29,28;,
  4;23,44,46,29;,
  4;24,25,31,30;,
  4;25,26,32,31;,
  4;26,27,33,32;,
  4;27,28,34,33;,
  4;28,29,35,34;,
  4;29,24,30,35;,
  3;47,48,49;,
  3;49,48,50;,
  3;48,51,50;,
  3;51,52,50;;
 }
 MeshTextureCoords {
  36;
  0.750000;0.125000;,
  0.875000;0.125000;,
  0.875000;0.125000;,
  0.750000;0.125000;,
  0.625000;0.125000;,
  0.625000;0.125000;,
  0.625000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  0.875000;0.250000;,
  0.875000;0.125000;,
  0.750000;0.125000;,
  0.625000;0.125000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.625000;0.125000;,
  0.750000;0.125000;,
  0.875000;0.250000;,
  0.875000;0.125000;,
  0.625000;0.125000;,
  0.750000;0.125000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  0.875000;0.125000;,
  0.625000;0.125000;,
  0.750000;0.125000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  0.875000;0.125000;;
 }
}