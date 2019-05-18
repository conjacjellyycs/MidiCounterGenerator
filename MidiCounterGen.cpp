//Midi Counter Generator by Conjac Jelly Charlieyan
//CJC Software Team (C) 2019-2019
#define titleversion "title Midi Counter Generator Version 2.0.1.1 by Conjac Jelly Charlieyan"

#include <bits/stdc++.h>
#include <windows.h>

//#ifdef WIN32
//#include <shlobj.h>
//#endif

using namespace std;
char junk;
int prec[65536];

long long tohex(const string& s){
	long long ha1=0,ha2=0;
	for(int i=0;i<s.size();i++){
		ha1=ha1*277+s[i];
		ha1%=1000000007;
		ha2=ha2*1003+s[i];
		ha2%=1000000009;
	}
	return ha1*1000000009+ha2;
}

char HexNum[]="0123456789ABCDEF";

string tofile(const string& s){
	long long x=tohex(s);
	string ans="";
	while(x){
		ans+=HexNum[x%16];
		x/=16;
	}
	ans="MidiCounterGen"+ans;
	return ans;
}

long long cntnc=0;
int lstcmd=256;

inline int getnxt(ifstream& inp){
	if(lstcmd!=256){
		int lff=lstcmd;
		lstcmd=256;
		return lff;
	}
	inp.read(&junk,1);
	int x=junk;
	if(x<0){
		x+=256;
	}
	cntnc++;
	return x;
}
#define go getnxt(inp)

inline long long getnum(ifstream& inp){
	long long x=0;
	int c;
	do{
		c=go;
		x=(x<<7)|(c&0b1111111);
	}while(c>=128);
	return x;
}
#define Go getnum(inp)

string inpfile,outfile;
vector<long long> trks;
vector<pair<long long,long double> > bpm;
vector<long double> tmc;
map<long long,int> mp;
bool died[65536];
long long tms[65536];
string filename;
vector<long long> dts;

int main(){
	system(titleversion); 
	cout<<"Input MIDI name: ";
	long long TM=clock();
	getline(cin,inpfile);
	if(inpfile[0]=='\"'){
		inpfile=inpfile.substr(1,inpfile.size()-2);
	}
	outfile='\"'+inpfile+".counter.mp4\"";
	ifstream inp(inpfile.c_str(),ios::binary);
	cout<<"Reading MIDI & counting notes...\n";
	/*char ff;
	inp.read(&ff,1);
	cout<<(int)ff<<endl;
	return 0;*/
	for(int i=0;i<10;i++){
		go;
	}
	int trkcnt=go;
	trkcnt=trkcnt*256+go;
	int resol=go;
	resol=resol*256+go;
	bpm.push_back(make_pair(0,500000.0/resol));
	mp[0]=0;
	cout<<"Track count: "<<trkcnt<<endl;
	long long notecnt=0;
	long long poly=0;
	for(int i=1;i<=trkcnt;i++){
		cout<<"Reading track "<<i<<"/"<<trkcnt<<", Size ";
		go;go;go;go;
		long long sz=go;
		//cerr<<inp.tellg()<<' '<<(sz=go)<<endl;
		sz=sz*256+go;
		sz=sz*256+go;
		sz=sz*256+go;
		cntnc=0;
		cout<<sz<<endl;
		lstcmd=256;
		long long TM=0;
		int prvcmd;
		bool start=0;
		while(1){
			TM+=Go;
			if(!start){
				tms[i]=TM;
				trks.push_back(inp.tellg());
				start=1;
			}
			int cmd=go;
			
			if(cmd<128){
				lstcmd=cmd;
				cmd=prvcmd;
			}
			prvcmd=cmd;
			int cm=cmd>>4<<4;
			if(cm==0b10010000){
				go;go;
				notecnt++;
			}else if(cm==0b10000000){
				go;go;
			}else if(cm==0b11000000||cm==0b11010000||cmd==0b11110011){
				go;
			}else if(cm==0b11100000||cm==0b10110000||cmd==0b11110010||cm==0b10100000){
				go;go;
			}else if(cmd==0b11110000){
				do{
				}while(go!=0b11110111);
			}else if(cmd==0b11110100||cmd==0b11110001||cmd==0b11110101||cmd==0b11111001||cmd==0b11111101||cmd==0b11110110||cmd==0b11110111||cmd==0b11111000||cmd==0b11111010||cmd==0b11111100||cmd==0b11111110){
			}else if(cmd==0b11111111){
				cmd=go;
				if(cmd==0){
					go;
				}else if(cmd>=1&&cmd<=10||cmd==0x7f){
					int ff=Go;
					while(ff--){
						go;
					}
				}else if(cmd==0x20||cmd==0x21){
					go;go;
				}else if(cmd==0x2f){
					go;
					break;
				}else if(cmd==0x51){
					go;
					int Bpm=go;
					Bpm=Bpm*256+go;
					Bpm=Bpm*256+go;
					//cerr<<TM<<' '<<Bpm*1.0/resol<<endl;
					if(mp.count(TM)){
						bpm[mp[TM]].second=((long double)Bpm)/resol;
					}else{
						bpm.push_back(make_pair(TM,((long double)Bpm)/resol));
						mp[TM]=bpm.size()-1;
					}
				}else if(cmd==0x54||cmd==0x58){
					go;go;go;go;go;
				}else if(cmd==0x59){
					go;go;go;
				}else if(cmd==0x0a){
					int ss=go;
					while(ss--){
						go;
					}
				}
			}
		}
		while(cntnc<sz){
			go;
		}
	}
	cout<<"Counting finished. Note count: "<<notecnt<<endl;
	mp.clear();
	sort(bpm.begin(),bpm.end());
	tmc.push_back(0);
	for(int i=1;i<bpm.size();i++){
		tmc.push_back(tmc[i-1]+(bpm[i].first-bpm[i-1].first)*bpm[i-1].second);
		//cerr<<tmc[i]<<endl;
	}
	int faq=trkcnt;
	int bpmptr=0;
	long double tmdf=0,tmm=0;
	long long allnotes=notecnt;
	notecnt=0;
	int frmcnt=0;
	system("echo %userprofile%>hauifwsfeiu.tmp");
	ifstream sfd("hauifwsfeiu.tmp");
	string tmpf;
	sfd>>tmpf;
	tmpf+="\\AppData\\Local\\Temp\\"+tofile(inpfile)+"\\";
	sfd.close();
	system("del hauifwsfeiu.tmp");
	system(("mkdir "+tmpf).c_str());
	ofstream ifs;
	while(faq){
		for(int i=0;i<trkcnt;i++){
			if(!died[i]){
				//cerr<<tms[i]<<endl;
				inp.seekg(trks[i]);
				bool bg=0;
				while(1){
					//long long back=inp.tellg(),ttt=tms[i];
					if(bg){
						tms[i]+=Go;
					}else{
						bg=1;
					}
					if(tms[i]>tmm){
						trks[i]=inp.tellg();
						//tms[i]=ttt;
						break;
					}
					int cmd=go;
					
					if(cmd<128){
						lstcmd=cmd;
						cmd=prec[i];
					}
					prec[i]=cmd;
					int cm=cmd>>4<<4;
					if(cm==0b10010000){
						go;go;
						notecnt++;
						poly++;
					}else if(cm==0b10000000){
						go;go;
						poly--;
					}else if(cm==0b11000000||cm==0b11010000||cmd==0b11110011){
						go;
					}else if(cm==0b11100000||cm==0b10110000||cmd==0b11110010||cm==0b10100000){
						go;go;
					}else if(cmd==0b11110000){
						do{
						}while(go!=0b11110111);
					}else if(cmd==0b11110100||cmd==0b11110001||cmd==0b11110101||cmd==0b11111001||cmd==0b11111101||cmd==0b11110110||cmd==0b11110111||cmd==0b11111000||cmd==0b11111010||cmd==0b11111100||cmd==0b11111110){
					}else if(cmd==0b11111111){
						cmd=go;
						if(cmd==0){
							go;
						}else if(cmd>=1&&cmd<=10||cmd==0x7f){
							int ff=Go;
							while(ff--){
								go;
							}
						}else if(cmd==0x20||cmd==0x21){
							go;go;
						}else if(cmd==0x2f){
							go;
							faq--;
							died[i]=1;
							break;
						}else if(cmd==0x51){
							go;go;go;go;
						}else if(cmd==0x54||cmd==0x58){
							go;go;go;go;go;
						}else if(cmd==0x59){
							go;go;go;
						}else if(cmd==0x0a){
							int ss=go;
							while(ss--){
								go;
							}
						}
					}
				}
				//cerr<<tms[i]<<' '<<tmm<<endl;
			}
		}
		stringstream ss;
		ss<<tmpf<<frmcnt<<endl;
		getline(ss,filename);
		ifs.open(filename.c_str());
		long long nps=notecnt-(dts.size()<25?0:dts[dts.size()-25]);
		dts.push_back(notecnt);
		ifs<<"NOTES: "<<notecnt<<'/'<<allnotes<<", NPS: "<<nps<<", Polyphony: "<<poly;
		ifs.close();
		tmdf+=40000;
		while(bpmptr<bpm.size()-1&&tmc[bpmptr+1]<tmdf){
			bpmptr++;
		}
		tmm=bpm[bpmptr].first+(tmdf-tmc[bpmptr])/bpm[bpmptr].second;
		cout<<"Generated frame "<<frmcnt<<", "<<fixed<<setprecision(2)<<frmcnt/25.0<<" secs, Notes: "<<notecnt<<", NPS: "<<nps<<", Polyphony: "<<poly<<endl;
		frmcnt++;
	}
	for(int i=0;i<frmcnt;i++){
		stringstream ss;
		cout<<"Converting frame "<<i<<" to picture..."<<endl;
		ss<<"echo "<<tmpf<<i<<"|python txt2png.py"<<endl;
		getline(ss,filename);
		system(filename.c_str());
	}
	cout<<"Outputting file..."<<endl;
	stringstream ss;
	ss<<"ffmpeg -f image2 -i \""<<tmpf<<"%d.png\" "<<outfile<<endl;
	getline(ss,filename);
	system(filename.c_str());
	stringstream sss;
	cout<<"Cleaning..."<<endl;
	sss<<"del "<<tmpf<<"* /q >nul"<<endl;
	getline(sss,filename);
	system(filename.c_str());
	stringstream ssss;
	ssss<<"rmdir "<<tmpf<<">nul"<<endl;
	getline(ssss,filename);
	system(filename.c_str());
	cout<<"Outputted file: "<<outfile<<endl;
	cout<<"Time used: "<<fixed<<setprecision(3)<<(double)(clock()-TM)/CLOCKS_PER_SEC<<" secs"<<endl;
	system("pause");
	return 0;
}
