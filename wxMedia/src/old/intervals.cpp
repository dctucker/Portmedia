/*
 *  intervals.cpp
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-05-03.
 *  Copyright 2010 UNCW. All rights reserved.
 *
 */

#include "intervals.h"



//Fraction ragisma( 4375., 4374. );

Interval IntervalTable[] = {
  {	Value(1.0), "Unison"
},{	Fraction(4375., 4374.), "ragisma"
},{	Fraction(2401., 2400.), "breedsma"
},{	Power( 2, Fraction( 1., 1200. ) ), "cent"
},{	Power( 2, Fraction( 1., 1000. ) ), "millioctave"
},{ Fraction( 32805.,32768. ), "schisma"
},{ Fraction( Power( 3, 53 ) , Power( 2, 84 ) ), "Mercator comma"
},{ Power( 10, Fraction( 1., 1000. ) ), "Savart"
},{ Fraction( 1224440064.,1220703125.), "parakleisma"
},{ Fraction( 225.,224. ), "septimal kleisma", "marvel comma"
},{ Fraction( 15625.,15552.), "kleisma"
},{ Fraction( 393216.,390625.), "Wuerschmidt comma"
},{ Fraction( 126.,125.), "septimal semicomma","starling comma"
},{ Fraction( 121, 120 )
},{ Fraction( 100, 99 )	
},{ Fraction( 99, 98 )	
},{ Fraction( 2048, 2025 ), "minor comma", "diaschisma"
},{ Fraction(81,80), "major comma","syntonic comma","comma of Didymus","(difference between four justly tuned perfect fifths and two octaves plus a major third) - There are 55.79763 syntonic commas in the octave"
},{ Power(2,Fraction(1,53)), "Arabian comma", "Holdrian comma","Holder's comma"
},{ Fraction(531441,524288),"Pythagorean comma","ditonic comma","(312/219)","(difference between twelve justly tuned perfect fifths and seven octaves) - There are 51.15087 Pythagorean commas in the octave"
},{ Fraction(65,64),"65th harmonic"
},{ Fraction(64,63),"septimal comma","comma of Archytas","","(the difference between the 3-limit or Pythagorean seventh and the harmonic seventh)"
},{ Fraction(20000,19683),"minimal diesis"
},{ Fraction(63,62)
},{ Fraction(3125,3072),"small diesis"
},{ Fraction(58,57)
},{ Fraction(57,56)
},{ Fraction(56,55),"Ptolemy's enharmonic"
},{ Fraction(55,54),
},{ Fraction(52,51),
},{ Fraction(51,50),
},{ Fraction(50,49),"septimal sixth-tone or jubilisma"
},{ Fraction(49,48),"septimal diesis or slendro diesis"
},{ Fraction(46,45),"inferior quarter-tone (Ptolemy)"
},{ Fraction(45,44)
//128,125
},{ Fraction(Fraction(16,15),Fraction(25,24)),"diminished second","enharmonic diesis","(the difference between three justly tuned major thirds and one octave), great diesis, enharmonic diesis (Vincentino), 5-limit diesis, limma"
},{ Fraction(525,512),"enharmonic diesis","Avicenna"
},{ Fraction(40,39),"difference between major and minor semitones"
},{ Fraction(39,38),"superior quarter-tone (Eratosthenes)"
},{ Fraction(77,75)
},{ Fraction(36,35),"superior or septimal quarter-tone"
},{ Fraction(250,243),"maximal diesis"
},{ Power(2,Fraction(1,24)),"equal-tempered quarter tone"
},{ Fraction(35,34),"equal temperament (ET) 1/4-tone approximation"
},{ Fraction(34,33)
},{ Fraction(50331648,48828125)
},{ Fraction(33,32),"33rd harmonic"
},{ Fraction(32,31),"inferior quarter-tone (Didymus)"
},{ Fraction(125,121)
},{ Fraction(31,30),"superior quarter-tone (Didymus)"
},{ Fraction(30,29)
},{ Fraction(29,28)
},{ Fraction(57,55)
},{ Fraction(648,625),"major diesis"
},{ Fraction(28,27),"inferior quarter-tone (Archytas)"
},{ Fraction(80,77)
},{ Fraction(27,26)
},{ Fraction(26,25),"1/3-tone (Avicenna)"
},{ Fraction(20480,19683),"comma that is associated with super-Pythagorean temperament"
},{ Fraction(51,49)	 
},{ Fraction(126,121)
},{ Fraction(25,24),"minor 5-limit semitone (half-step)", "chromatic diesis", "semitone minimus", "lesser or just chromatic semitone, minor chroma"
},{ Fraction(24,23)
},{ Fraction(117,112)
},{ Fraction(23,22)
},{ Fraction(67,64),"67th harmonic"
},{ Fraction(22,21),"hard semitone (1/2-step) (Ptolemy, Avicenna, Safiud)"
},{ Fraction(21,20),"major diesis, septimal chromatic semitone"
},{ Fraction(81,77)
},{ Fraction(20,19)
},{ Fraction(256,243),"Pythagorean diatonic semitone (half-step), minor or Pythagorean limma, minor semitone"
},{ Fraction(58,55)
},{ Fraction(135,128), "limma ascendant", "greater chromatic semitone", "semitone medius", "chromatic semitone, major chroma"
},{ Fraction(96,91)
},{ Fraction(6442450944.,6103515625.)
},{ Fraction(19,18)
},{ Fraction(55,52)
},{ Fraction(128,121)
},{ Fraction(18,17),"equal temperament (ET) semitone (half-step) (approximation familiar to makers of lutes, who used it when fixing frets on lute fingerboards)"
},{ Power(2,Fraction(1,12)),"equal temperament (ET) semitone (half-step)", "equal-tempered minor second, exact"
},{	Fraction(35,33)
},{	Fraction(52,49)
},{	Fraction(86,81)
},{	Fraction(17,16),"overtone semitone (half-step)"
},{	Fraction(33,31)
},{	Fraction(49,46)
},{	Fraction(16,15),"major 5-limit semitone (half-step)", "just diatonic semitone"
},{	Fraction(2187,2048),"apotome or apotome Pythagorica", "Pythagorean major semitone", "Pythagorean chromatic semitone","the difference between a Pythagorean diatonic semitone and a Pythagorean chromatic semitone is the Pythagorean or ditonic comma"
},{	Fraction(31,29)
},{	Fraction(77,72)
},{	Fraction(15,14),"Cowell just semitone (half-step)", "septimal diatonic semitone"
},{	Fraction(29,27)
},{	Fraction(14,13),"major semitone"
},{	Fraction(69,64),"69th harmonic"
},{	Fraction(55,51)
},{	Fraction(27,25),"alternate Renaissance semitone (half-step)", "semitone maximus", "minor second", "large limma"
},{	Fraction(121,112)
},{	Fraction(13,12),"3/4-tone (Avicenna)","minor tone"
},{	Fraction(64,59)
},{	Fraction(38,35)
},{	Fraction(63,58)
},{	Fraction(88,81)
},{	Fraction(25,23)
},{	Fraction(62,57)
},{	Fraction(135,124)
},{	Fraction(49,45)
},{	Power(2, Fraction(3,24)),"Equal tempered Neutral second"
},{	Fraction(12,11),"undecimal 'median' semitone (1/2-step)","lesser undecimal neutral second"
},{	Fraction(59,54)
},{	Fraction(35,32),"35th harmonic"
},{	Fraction(23,21)
},{	Fraction(57,52)
},{	Fraction(34,31)
},{	Fraction(800,729)
},{	Fraction(56,51)
},{	Fraction(11,10),"greater undemical neutral second"
},{	Fraction(54,49)
},{	Fraction(32,29)

  }
};

/*
(21:19)	173.268	 
(31:28)	176.210	 
(567:512)	176.646	 
(51:46)	178.636	 
(71:64)	179.697	71st harmonic
(10:9)	182.404	minor whole-tone, just minor tone, smaller step
(49:40)	186.338	 
(39:35)	187.343	 
(29:26)	189.050	 
(125:112)	190.115	 
(48:43)	190.437	 
(19:17)	192.558	 
(160:143)	194.468	 
(28:25)	196.198	 
(121:108)	196.771	 
(55:49)	199.980	 
(2^1/6:1)	200.000	equal-tempered whole-tone, exact, equal-tempered major second
(64:57)	200.532	 
(9:8)	203.910	major whole-tone, major tone, greater step, just major second, sesquioctave, tonus, 5th harmonic (normalised)
(62:55)	207.404	 
(44:39)	208.835	 
(35:31)	210.104	 
(26:23)	212.253	 
(112:99)	213.598	 
(17:15)	216.687	 
(25:22)	221.309	 
(58:51)	222.667	 
(256:225)	222.463	 
(33:29)	223.696	 
(729:640)	225.416	 
(57:50)	226.841	 
(73:64)	227.789	73rd harmonic
(8:7)	231.174	septimal whole-tone, septimal major second
(63:55)	235.104	 
(55:48)	235.677	 
(39:34)	237.527	 
(225:196)	238.886	 
(31:27)	239.171	 
(147:128)	239.607	 
(169:147)	241.449	 
(23:20)	241.961	 
(2187:1900)	243.545	 
(38:33)	244.240	 
(144:125)	244.969	diminished third (6/5 x 24/25)
(121:105)	245.541	 
(15:13)	247.741	 
(52:45)	250.304	 
(37:32)	251.344	37th harmonic
(81:70)	252.680	 
(125:108)	253.076	 
(22:19)	253.805	 
(51:44)	255.592	 
(196:169)	256.596	consonant interval (Avicenna)
(29:25)	256.950	 
(36:31)	258.874	 
(93:80)	260.677	 
(57:49)	261.816	 
(64:55)	262.368	 
(7:6)	266.871	septimal minor third, subminor third
the named interval is only approximately equal to 7:6 frequency ratio. In terms of cents, it is 267 cents, a quartertone of size 36/35 flatter than a 5-limit just minor third of 6:5. In the meantone era the interval made its appearance as the alternative minor third in remote keys, under the name augmented second
(90:77)	270.080	 
(75:64)	274.582	augmented second (9/8 x 25/24)
(34:29)	275.378	 
(88:75)	276.736	 
(27:23)	277.591	 
(20:17)	281.358	 
(33:28)	284.447	 
(46:39)	285.792	 
(13:11)	289.210	 
(58:49)	291.925	 
(45:38)	292.711	 
(32:27)	294.135	Pythagorean minor third, Pythagorean semiditone
(19:16)	297.513	overtone minor third
(2^1/4:1)	300.000	equal-tempered minor third, exact
(25:21)	301.847	 
(31:26)	304.508	 
(105:88)	305.777	 
(55:46)	309.357	 
(6:5)	315.641	5-limit just minor third, sesquiquintan, semiditonus
(19683:16384)	317.595	 
(77:64)	320.144	77th harmonic
(35:29)	325.562	 
(29:24)	327.622	 
(75:62)	329.547	 
(98:81)	329.832	 
(121:100)	330.008	 
(23:19)	330.761	 
(63:52)	332.208	 
(40:33)	333.041	 
(17:14)	336.130	 
(243:200)	337.148	 
(62:51)	338.125	 
(28:23)	340.552	 
(39:32)	342.483	39th harmonic (major third minus a minor diesis)
(128:105)	342.905	 
(8000:6561)	343.301	 
(11:9)	347.408	undecimal "median" third, undecimal neutral third
(2^7/24)	350.000	equal tempered neutral third
(60:49)	350.617	 
(49:40)	351.338	 
(38:31)	352.477	 
(27:22)	354.547	 
(16:13)	359.472	 
(79:64)	364.537	79th harmonic
(100:81)	364.807	 
(121:98)	364.984	 
(21:17)	365.825	 
(99:80)	368.914	 
(26:21)	369.747	 
(57:46)	371.194	 
(31:25)	372.408	 
(36:29)	374.333	 
(56:45)	378.602	 
(96:77)	381.811	 
(8192:6561)	384.360	Pythagorean "schismatic" third
(5:4)	386.314	5-limit just major third, sesquiquartan, ditonus
(64:51)	393.090	 
(49:39)	395.169	 
(44:35)	396.178	 
(39:31)	397.447	 
(34:27)	399.090	 
(2^1/3:1)	400.000	equal-tempered major third, exact
(63:50)	400.108	 
(121:96)	400.681	 
(29:23)	401.303	 
(125:99)	403.713	 
(24:19)	404.442	 
(512:405)	405.866	 
(62:49)	407.384	 
(81:64)	407.820	Pythagorean major third, Pythagorean ditone (that is two 9:8 tones)
(19:15)	409.244	 
(33:26)	412.745	 
(80:63)	413.578	 
(14:11)	417.508	undecimal major third
(51:40)	420.597	 
(125:98)	421.289	 
(23:18)	424.364	 
(32:25)	427.373	diminished fourth
(41:32)	429.062	41st harmonic
(50:39)	430.145	major third plus a minor diesis
(77:60)	431.875	 
(9:7)	435.084	septimal major third, supermajor third - the named interval interval is exactly or approximately equal to a 9:7 frequency ratio. In terms of cents, it is 435 cents, a quartertone of size 36/35 sharper than a just major third of 5/4. In the early meantone era the interval made its appearance as the alternative major third in remote keys, under the name diminished fourth.
(58:45)	439.353	 
(49:38)	440.139	 
(40:31)	441.278	 
(31:24)	443.081	 
(1323:1024)	443.517	 
(128:99)	444.772	 
(22:17)	446.363	 
(57:44)	448.150	 
(162:125)	448.879	 
(35:27)	449.275	 
(83:64)	450.047	83rd harmonic
(100:77)	452.484	 
(13:10)	454.214	perfect fourth minus a minor diesis
(125:96)	456.986	augmented third (5/4 x 25/24)
(30:23)	459.994	 
(64:49)	462.348	 
(98:75)	463.069	 
(17:13)	464.428	 
(72:55)	466.278	 
(55:42)	466.851	 
(38:29)	467.936	 
(21:16)	470.781	septimal fourth
(46:35)	473.135	 
(25:19)	475.114	 
(320:243)	476.539	 
(29:22)	478.259	 
(675:512)	478.492	 
(33:25)	480.646	 
(45:34)	485.286	 
(85:64)	491.269	85th harmonic
(4:3)	498.045	harmonic fourth, just perfect fourth, sesquitertan, sesquitertium, diatessaron
(2^5/12:1)	500.000	equal-tempered perfect fourth, exact
(75:56)	505.757	 
(51:38)	509.397	 
(43:32)	511.518	43rd harmonic
(121:90)	512.412	 
(39:29)	512.905	 
(35:26)	514.612	 
(66:49)	515.621	 
(31:23)	516.761	 
(27:20)	519.551	5-limit wolf fourth
(177147:131072)	521.505	 
(23:17)	523.319	 
(42:31)	525.745	 
(19:14)	528.687	 
(110:81)	529.812	 
(87:64)	531.532	87th harmonic
(34:25)	532.328	 
(49:36)	533.742	 
(15:11)	536.951	 
(512:375)	539.104	 
(160/117)	541.9	perfect fourth plus a minor diesis
(26:19)	543.015	 
(63:46)	544.462	 
(48:35)	546.815	 
(1000:729)	547.211	 
(11:8)	551.318	undecimal tritone (11th harmonic)
(62:45)	554.812	 
(40:29)	556.737	 
(29:21)	558.796	 
(112:81)	561.006	 
(18:13)	563.382	 
(25:18)	568.717	augmented fourth (4/3 x 25/24)
(89:64)	570.880	89th harmonic
(32:23)	571.726	 
(39:28)	573.657	 
(46:33)	575.001	 
(88:63)	578.582	 
(7:5)	582.512	lesser septimal tritone
(108:77)	585.721	 
(1024:729)	588.270	low Pythagorean tritone
(45:32)	590.224	high 5-limit tritone (major third plus 9/8 whole tone) (25:18 if the 10/9 whole tone used instead)
(38:27)	591.648	 
(31:22)	593.718	 
(55:39)	595.149	 
(24:17)	597.000	 
(2^1/2:1)	600.000	equal-tempered tritone, exact
(99:70)	600.088	 
(17:12)	603.000	 
(44:31)	606.282	 
(125:88)	607.623	 
(27:19)	608.352	 
(91:64)	609.354	91st harmonic
(64:45)	609.776	low 5-limit tritone
(729:512)	611.730	high Pythagorean tritone, Tritonus (German)
(57:40)	613.154	 
(77:54)	614.279	 
(10:7)	617.488	greater septimal tritone
(63:44)	621.418	 
(33:23)	624.999	 
(56:39)	626.343	 
(23:16)	628.274	23rd harmonic
(36:25)	631.283	diminished fifth (3/2 x 24/25)
(121:84)	631.855	 
(49:34)	632.696	 
(75:52)	634.1	diminished fifth (tritone plus a minor diesis)
(13:9)	636.618	 
(81:56)	638.994	 
(55:38)	640.119	 
(42:29)	641.204	 
(29:20)	643.263	 
(45:31)	645.188	 
(93:64)	646.991	93rd harmonic
(16:11)	648.682	inversion of eleventh harmonic
(51:35)	651.771	 
(729:500)	652.789	 
(35:24)	653.185	 
(19:13)	656.985	 
(117:80)	658.1	perfect fifth minus a minor diesis
(375:256)	660.896	 
(22:15)	663.049	 
(47:32)	665.507	47th harmonic
(72:49)	666.258	 
(25:17)	667.672	 
(81:55)	670.188	 
(28:19)	671.313	 
(31:21)	674.255	 
(189:128)	674.691	 
(34:23)	676.681	 
(262144:177147)	678.495	wolf fifth, the interval between G# and Ed in early Pythagorean chromatic tuning, a fifth containing (i.e. narrowed by) the whole of a Pythagorean or ditonic comma (531441:524288)
(40:27)	680.449	(dissonant) 5-limit wolf fifth
(46:31)	683.239	 
(95:64)	683.827	95th harmonic
(49:33)	684.379	 
(52:35)	685.388	 
(58:39)	687.095	 
(125:84)	688.160	 
(112:75)	694.243	 
(121:81)	694.816	 
(2^7/12:1)	700.000	equal-tempered perfect fifth, exact
(3:2)	701.955	harmonic fifth, just perfect fifth, trihemitone, diapente, sesquialterum, 3rd harmonic
(121:80)	716.322	 
(50:33)	719.354	 
(97:64)	719.895	97th harmonic
(1024:675)	721.508	 
(44:29)	721.741	 
(243:160)	723.461	 
(38:25)	724.886	 
(35:23)	726.865	 
(32:21)	729.219	 
(29:19)	732.064	 
(84:55)	733.149	 
(55:36)	733.722	 
(26:17)	735.572	 
(75:49)	736.931	 
(49:32)	737.652	49th harmonic
(23:15)	740.006	 
(192:125)	743.014	diminished sixth (8/5 x 24/25)
(20:13)	745.786	perfect fifth plus a minor diesis
(77:50)	747.516	 
(54:35)	750.725	 
(125:81)	751.121	 
(17:11)	753.637	 
(99:64)	755.228	99th harmonic
(48:31)	756.919	 
(31:20)	758.722	 
(45:29)	760.647	 
(14:9)	764.916	septimal minor sixth
(120:77)	768.125	 
(39:25)	769.855	minor sixth minus a minor diesis
(25:16)	772.627	augmented fifth
(36:23)	775.636	 
(11:7)	782.492	undecimal minor sixth
(63:40)	786.422	 
(52:33)	787.255	 
(101:64)	789.854	101st harmonic
(30:19)	790.756	 
(128:81)	792.180	Pythagorean minor sixth
(49:31)	792.616	 
(405:256)	794.134	 
(19:12)	795.558	 
(46:29)	798.697	 
(100:63)	799.892	 
(2^2/3:1)	800.000	equal-tempered minor sixth, exact
(27:17)	800.910	 
(62:39)	802.553	 
(35:22)	803.822	 
(51:32)	806.910	51st harmonic
(8:5)	813.686	5-limit just minor sixth (inversion of 5-limit just major third)
(6561:4096)	815.640	Pythagorean "schismatic" sixth
(77:48)	818.189	 
(45:28)	821.398	 
(103:64)	823.801	103rd harmonic
(29:18)	825.667	 
(50:31)	827.592	 
(121:75)	828.053	 
(21:13)	830.253	 
(55:34)	832.676	 
(34:21)	834.175	 
(81:50)	835.193	 
(125:77)	838.797	 
(13:8)	840.528	overtone sixth
(57:35)	844.328	 
(44:27)	845.453	 
(31:19)	847.523	 
(80:49)	848.662	 
(49:30)	849.383	 
(18:11)	852.592	undecimal "median" sixth, undecimal neutral sixth
(105:64)	857.095	105th harmonic
(64:39)	857.517	minor sixth plus a minor diesis
(23:14)	859.448	 
(51:31)	861.875	 
(400:243)	862.852	 
(28:17)	863.870	 
(33:20)	866.959	 
(38:23)	869.239	 
(81:49)	870.168	 
(48:29)	872.378	 
(53:32)	873.505	53rd harmonic
(58:35)	874.438	 
(63:38)	875.223	 
(128:77)	879.856	 
(107:64)	889.760	107th harmonic
(5:3)	884.359	5-limit just major sixth (inversion of a 5-limit just minor third)
(57:34)	894.513	 
(52:31)	895.492	 
(42:25)	898.153	 
(121:72)	898.726	 
(23/4:1)	900.000	equal-tempered major sixth, exact
(32:19)	902.487	 
(27:16)	905.865	Pythagorean major sixth
(49:29)	908.075	 
(22:13)	910.790	 
(39:23)	914.208	 
(56:33)	915.553	 
(17:10)	918.642	 
(109:64)	921.821	109th harmonic
(46:27)	922.409	 
(75:44)	923.264	 
(29:17)	924.622	 
(128:75)	925.418	diminished seventh (16/9 x 24/25)
(77:45)	929.920	 
(12:7)	933.129	septimal major sixth (inversion of a minimal third)
(55:32)	937.632	55th harmonic
(31:18)	941.126	 
(441:256)	941.562	 
(50:29)	943.050	 
(19:11)	946.195	 
(216:125)	946.924	 
(121:70)	947.496	 
(45:26)	949.696	 
(26:15)	952.259	 
(111:64)	953.299	111th harmonic
(125:72)	955.031	augmented sixth (5/3 x 25/24)
(33:19)	955.760	 
(40:23)	958.039	 
(54:31)	960.829	 
(96:55)	964.323	 
(110:63)	964.896	 
(7:4)	968.826	septimal minor seventh, harmonic seventh, (inversion of major tone) [some assert that 7:4 is one of the blue notes used in jazz]
(58:33)	976.304	 
(225:128)	976.537	225th harmonic
(51:29)	977.333	 
(44:25)	978.691	 
(30:17)	983.313	 
(113:64)	984.215	113th harmonic
(99:56)	986.402	 
(23:13)	987.747	 
(62:35)	989.896	 
(39:22)	991.165	 
(55:31)	992.596	 
(16:9)	996.090	Pythagorean small minor seventh, lesser just minor seventh, dominant seventh (one of the inversions of a whole tone, the other being 9:5)
(57:32)	999.468	57th harmonic
(2^5/6:1)	1000.000	equal-tempered minor seventh
(98:55)	1000.020	 
(25:14)	1003.802	 
(34:19)	1007.442	 
(52:29)	1010.950	 
(88:49)	1013.666	 
(115:64)	1014.588	115th harmonic
(9:5)	1017.596	5-limit large minor seventh, greater just minor seventh, tonic seventh (one of the inversions of a whole tone, the other being 16:9)
(59049:32768)	1019.550	 
(56:31)	1023.790	 
(38:21)	1026.732	 
(29:16)	1029.577	29th harmonic
(49:27)	1031.787	 
(20:11)	1034.996	lesser undecimal neutral seventh
(51:28)	1038.085	 
(729:400)	1039.103	 
(31:17)	1040.080	 
(42:23)	1042.507	 
(117:64)	1044.438	117th harmonic
(64:35)	1044.860	 
(4000:2187)	1045.256	 
(11:6)	1049.363	undecimal "median" seventh, greater undecimal neutral seventh
(90:49)	1052.572	 
(57:31)	1054.432	 
(46:25)	1055.647	 
(81:44)	1056.502	 
(35:19)	1057.627	 
(59:32)	1059.172	59th harmonic
(24:13)	1061.427	inversion of minor tone
(50:27)	1066.762	 
(63:34)	1067.780	 
(13:7)	1071.702	inversion of major semitone
(119:64)	1073.781	119th harmonic
(54:29)	1076.288	 
(28:15)	1080.557	 
(58:31)	1084.542	 
(15:8)	1088.269	5-limit just major seventh
(62:33)	1091.763	 
(32:17)	1095.045	 
(49:26)	1097.124	 
(66:35)	1098.133	 
(2^11/12:1)	1100.000	equal-tempered major seventh, exact
(17:9)	1101.045	 
(121:64)	1102.636	121st harmonic
(125:66)	1105.668	 
(36:19)	1106.397	 
(256:135)	1107.821	 
(55:29)	1108.054	 
(243:128)	1109.775	Pythagorean major seventh
(19:10)	1111.199	 
(40:21)	1115.533	inversion of minor semitone/major diesis
(61:32)	1116.885	61st harmonic
(21:11)	1119.463	 
(44:23)	1123.044	 
(23:12)	1126.319	 
(48:25)	1129.328	diminished octave
(121:63)	1129.900	 
(123:64)	1131.017	123rd harmonic
(25:13)	1132.100	 
(77:40)	1133.830	 
(52:27)	1134.663	 
(27:14)	1137.039	septimal major seventh
(56:29)	1139.249	 
(29:15)	1141.308	 
(60:31)	1143.233	 
(31:16)	1145.036	31st harmonic
(64:33)	1146.727	 
(33:17)	1148.318	 
(243:125)	1150.834	 
(39:20)	1156.169	inversion of minor diesis
(125:64)	1158.941	augmented seventh (15/8 x 25/24)
(88:45)	1161.094	 
(45:23)	1161.991	 
(96:49)	1164.303	 
(49:25)	1165.024	 
(51:26)	1166.383	 
(108:55)	1168.233	 
(55:28)	1168.806	 
(57:29)	1169.891	 
(63:32)	1172.736	63rd harmonic
(160:81)	1178.494	 
(99:50)	1182.601	 
(125:63)	1186.205	 
(127:64)	1186.422	127th harmonic
(2:1)	1200.000	octave, diapason, 2nd harmonic (which normalizes to 1:1, i.e. the 1st harmonic or fundamental) useful intervals that exceed the octave
(32:15)	1311.731	minor ninth
(9:4)	1403.910	major ninth
(7:3)	1466.871	harmonic minor tenth
(12:5)	1515.641	minor ninth
(5:2)	1586.314	major ninth
(8:3)	1698.045	perfect eleventh
(11:4)	1751.318	harmonic eleventh
(45:16)	1790.224	augmented eleventh
(3:1)	1901.955	just perfect twelfth, tritave
(25:8)	1972.627	augmented twelfth
(16:5)	2013:686	minor thirteenth
(13:4)	2040.528	harmonic thirteenth
(10:3)	2084.359	major thirteenth
(7:2)	2168.826	harmonic fourteenth
(32:9)	2196.090	dominant thirteenth
(18:5)	2217.596	tonic fourteenth
(15:4)	2288.269	major fourteenth
(4:1)	2400.000	double octave, fifteenth

//*/

