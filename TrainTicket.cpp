// TrainTicketSelling.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <windows.h> 
#include <wincon.h>
#include<vector>
using namespace std;

int TotalStation = 0;
int TotalSeat = 0;
int **SeatStatusMatrix;// 0:for sell 1:sold
int EventNo = 0;// selling record

class CSet
{
private:
	int RealSeatAmout = 0;
public:
	int *CSetSpec;
	void initialSeatBit(int TreeX, int Cx, int Amount)
	{
		RealSeatAmout = Amount;
		CSetSpec = new int[RealSeatAmout];
		if (TreeX == 0 && Cx == TotalStation - 1)// T0's CTotalSeat has all Seats in this period
		{
			for (int i = 0; i < RealSeatAmout; i++)
			{
				CSetSpec[i] = i + 1;
			};
		}
		else
		{
			for (int i = 0; i < RealSeatAmout; i++)
			{
				CSetSpec[i] = 0;
			};
		}
	}
	int* getCSetSpec()
	{
		return CSetSpec;
	}
	int getCSetSpecLength()
	{
		return RealSeatAmout;
	}
	void deleteBit(int ChangeBit)
	{
		CSetSpec[ChangeBit] = 0;// sold
	}
	void addBit(int ChangeBit)
	{
		CSetSpec[ChangeBit] = ChangeBit + 1;// for sell
	}
};

class TreeCluster
{
private:
	int CStationMax = TotalStation;
public:
	CSet *TreeClusterSetSpec;
	void buildTree(int Order)// No.Order
	{
		CStationMax = TotalStation - Order;// Ti has maximum of CStationMax nodes
		TreeClusterSetSpec = new CSet[CStationMax];
		for (int i = 0; i < CStationMax; i++)
		{
			CSet CSet;// Each CSet node has a maximum of TotalSeat bits, compress later
			CSet.initialSeatBit(Order, i, TotalSeat);// Tree Oder's Ci
			TreeClusterSetSpec[i] = CSet;
		}
	}
	int getCLength()
	{
		return CStationMax;
	}
	CSet* getCSet()
	{
		return TreeClusterSetSpec;
	}
};

TreeCluster* TreeClusterSet;//TreeCluster，Inner C Set



class InitialTree
{
private:
	int CStationMax = 0;

public:
	InitialTree() {
		SeatStatusMatrix = new int*[TotalSeat];// Seats & Station
		for (int i = 0; i<TotalSeat; i++)
		{
			SeatStatusMatrix[i] = new int[TotalStation];
		};
		for (int i = 0; i < TotalSeat; i++)
		{
			for (int j = 0; j < TotalStation; j++)
			{
				SeatStatusMatrix[i][j] = 0;
			};
		};

		TreeClusterSet = new TreeCluster[TotalStation];// TotalStation Trees
		for (int i = 0; i < TotalStation; i++)
		{
			TreeCluster TreeX;// initialize each tree
			TreeX.buildTree(i);// Tree i
			TreeClusterSet[i] = TreeX;
		}
	}
	void InitializeResult()
	{
		cout << "We Have " << TotalStation << " Stations and " << TotalSeat << " Seats to Start the Journey!" << endl;
		cout << "Let's Go! " << endl << "~ Wu ~ Wu ~ Wu ~" << endl;
	}
};

class PrintTree
{
public:
	PrintTree()
	{
		for (int i = 0; i < TotalSeat + 1; i++)
		{
			for (int j = 0; j < TotalStation + 1; j++)
			{
				if (i == 0)
				{
					if (j == 0)
					{
						cout << setw(4) << right << "X" << setw(3) << "";
					}
					else
					{
						cout << setw(2) << right << "St" << setw(3) << right << j - 1 << "-" << setw(3) << left << j;
					}
				}
				else if (j == 0)
				{
					cout << setw(4) << left << "Seat" << setw(3) << left << i;
				}
				else
				{
					if (SeatStatusMatrix[i - 1][j - 1] == 0)
					{
						cout << setw(4) << left << "" << setw(1) << left << "F" << setw(4) << "";
					}
					else
					{
						cout << setw(4) << left << "" << setw(1) << left << "T" << setw(4) << "";
					}
				}
			}
			cout << endl;
		};
		cout << "The Tree Cluster is Shown as Below:" << endl;

		for (int i = 0; i < TotalStation; i++)// i Stations
		{
			cout << "-- Tree " << i << "--" << endl;
			TreeCluster CurrentTreeCluster = TreeClusterSet[i];
			for (int j = 0; j < CurrentTreeCluster.getCLength(); j++)// 
			{
				cout << "CSet " << (j + 1) << ":";
				CSet *CurrentCSet;
				CurrentCSet = CurrentTreeCluster.getCSet();
				int *CSetSpec = CurrentCSet[j].getCSetSpec();
				int SizeofCSetSpec = CurrentCSet[j].getCSetSpecLength();
				for (int k = 0; k < SizeofCSetSpec; k++)
				{
					cout << setw(3) << right << CSetSpec[k];
				};
				cout << endl;
			};
		};
	}
};

class TicketDetails
{
private:
	int TreeNo = 0;
	int CNo = 0;
	int SeatNo = 0;
public:
	void setSold(int Tree, int C, int Seat)
	{
		TreeNo = Tree;
		CNo = C;
		SeatNo = Seat;
	}
	int getTreeNo()
	{
		return TreeNo;
	}
	int getCNo()
	{
		return CNo;
	}
	int getSeatNo()
	{
		return SeatNo;
	}
};

class SoldTicketLog
{
private:
	int CurrentEventNo = 0;
	TicketDetails CurrentTD;
public:
	void addSoldTicket(TicketDetails TD)
	{
		CurrentEventNo = EventNo++;
		CurrentTD = TD;
	}
	int getEventNo()
	{
		return CurrentEventNo;
	}
	TicketDetails getTicketDetails()
	{
		return CurrentTD;
	}
};

vector<SoldTicketLog> SoldTicketLogList;// sold tickets

class BuyTicket
{
private:
	int CurrentTicketAmout = 0;
	int CurrentStartStation = 0;
	int CurrentEndStation = 0;
	//int TicketJump = 0;// afterward jump
	int StationStep = 0;// foreward step
	int InternalStation = 0;// End - Start
	int Looking = 0;// Looking for
	int Found = 0;// Already Found
	int CStart = 0;//Search from CStart-th Node
	int CurrentTreeNo = 0;
public:
	vector<TicketDetails> TicketList;

	void searchTree(TreeCluster TargetTree, int TreeNo)
	{
		CSet* CurrentCSetList = TargetTree.getCSet();
		CurrentTreeNo = TreeNo;
		//TargetTree.getCLength(): Current Tree has Maximum of CMax Nodes
		for (int i = CStart - 1; i < TargetTree.getCLength(); i++)
		{
			if (Looking > 0)
			{
				CSet CurrentCSet = CurrentCSetList[i];
				int* CurrentCSetSpec = CurrentCSet.getCSetSpec();
				int CurrentCSetSpecSize = CurrentCSet.getCSetSpecLength();
				for (int j = 0; j < CurrentCSetSpecSize; j++)
				{
					if (Looking > 0)
					{
						if (CurrentCSetSpec[j] != 0)// For Sell
						{
							Found++;
							Looking--;
							TicketDetails TD;
							TD.setSold(CurrentTreeNo, i, j);// Ticket: CurrentTree,Ci,Seatj
							TicketList.push_back(TD);
						}
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
	}

	// numbers are valid before enter, ticketAmount should be calculated after requesting
	vector<TicketDetails> getTicketList(int TicketAmount, int StartStation, int EndStation)
	{
		CurrentStartStation = StartStation;
		CurrentEndStation = EndStation;
		CurrentTicketAmout = TicketAmount;
		InternalStation = CurrentEndStation - CurrentStartStation;
		Looking = TicketAmount;
		TreeCluster CurrentTree = TreeClusterSet[CurrentStartStation];
		CStart = InternalStation + StationStep;// Initially: InternalStation
		CurrentTreeNo = CurrentStartStation;
		while (1)
		{
			searchTree(CurrentTree, CurrentTreeNo);
			cout << "CurrentTreeNo:" << CurrentTreeNo << endl;
			if (Found < CurrentTicketAmout)
			{
				StationStep++;// StationStep + 1, then CStart + 1
				CStart++;
				CurrentTreeNo = CurrentStartStation - StationStep;
				if (!(CurrentStartStation - StationStep < 0))
				{
					CurrentTree = TreeClusterSet[CurrentTreeNo];
				}
				else
				{
					cout << "Sorry, Sold Out." << endl;// cannot find the result till Tree0
					break;
				}
			}
			else //Found == CurrentTicketAmout
			{
				cout << "Your Tickets Have been Issued" << endl;
				break;
			}
		}
		return TicketList;
	}
};

class UpdateTree
{
private:
	int Step = 0;
	int Jump = 0;
	TreeCluster TreeStep;
	CSet CStepJumpInterval;// TreeStep CStepJumpInterval delete SeatNo
	CSet CStep;// TreeStep CStep add SeatNo
	TreeCluster TreeEndStation;
	CSet CJump;// TreeEndStation CJump add SeatNo
public:
	void setBit(int TreeNo, int CNo, int SeatNo, int Start, int End)// maximum of 3 nodes
	{
		TreeStep = TreeClusterSet[TreeNo];
		CStepJumpInterval = TreeStep.getCSet()[CNo];
		CStepJumpInterval.deleteBit(SeatNo);
		Step = Start - TreeNo;
		if (Step > 0)
		{
			CStep = TreeStep.getCSet()[Step - 1];
			CStep.addBit(SeatNo);
		}
		Jump = CNo + 1 - (End - Start) - Step;
		cout << "---- Step:" << Step << ",Jump:" << Jump << ",SeatNo:" << (SeatNo + 1) << endl;
		if (Jump > 0)
		{
			TreeEndStation = TreeClusterSet[End];
			CJump = TreeEndStation.getCSet()[Jump - 1];
			// cout << CJump.getCSetSpec()[Jump - 1];
			CJump.addBit(SeatNo);
		}
		if (Jump < 0)
		{
			cout << "ERROR, TreeNo: " << TreeNo << ", CNo:" << CNo << ", SeatNo" << SeatNo << ", Start:" << Start << ", End:" << End << endl;
		}
	}
	void refundBit(int TreeNo, int CNo, int SeatNo)// TreeNo, CNo add SeatNo
	{
		TreeCluster TC = TreeClusterSet[TreeNo];
		CSet RCSet = TC.getCSet()[CNo];
		RCSet.addBit(SeatNo);
	}
};

class UpdateMatrix
{
public:
	void setSoldTicket(int Start, int End, int SeatNo)
	{
		for (int i = Start; i < End; i++)
		{
			SeatStatusMatrix[SeatNo][i] = 1;
		}
	}
	void setRefundTicket(int Start, int End, int SeatNo)
	{
		for (int i = Start; i < End; i++)
		{
			SeatStatusMatrix[SeatNo][i] = 0;
		}
	}
};

class CheckInput
{
private:
	int Operation = 0;
	int StartStation = 0;
	int EndStation = 0;
	int TicketAmount = 0;
public:
	bool getCheckingResult()
	{
		bool Next = 1;
		cout << "Please Select an Operation" << endl << "1: Show Current Seat Status" << endl << "2: Buy Tickets" << endl << "3: Refund" << endl << "4: Exit" << endl;
		cin >> Operation;
		switch (Operation)
		{
		case 1:
		{
			cout << "The Current Status of Seats is Shown as Below:" << endl;
			PrintTree PT;
			break;
		}
		case 2:
		{
			cout << "Welcome to Use \"KICKTICKET\" to Buy Train Tickets" << endl;
			cout << "Please Select a StartStaion in Number(0-" << (TotalStation - 1) << "):";
			cin >> StartStation;
			if (StartStation < 0 || StartStation >(TotalStation - 1))
			{
				cout << "Invalid StartStation!" << endl;
				break;
			}
			cout << "Please Select an EndStation in Number(1-" << TotalStation << "):";
			cin >> EndStation;
			if (EndStation < 1 || EndStation > TotalStation)
			{
				cout << "Invalid EndStation!" << endl;
				break;
			}
			else if (!(EndStation > StartStation))
			{
				cout << "R U Kidding Me? You R On the Opposite Direction Train" << endl;
				break;
			}
			cout << "How Many Train Tickets Do You Want(1-" << TotalSeat << "):";
			cin >> TicketAmount;
			if (!(TicketAmount > 0) || TicketAmount > TotalSeat)
			{
				cout << "R U Kidding Me? TicketAmout Should be Greater Than 0 and Less Than " << TotalSeat << "!" << endl;
				break;
			}
			cout << "You Gonna Start From Station " << StartStation << ", Arrive at Station " << EndStation << ", and " << TicketAmount << " Tickets in Total" << endl;
			cout << "Les gar";
			printf("\x87");
			cout << "on chanter: H\x82l\x8Ane, Ce Train Qui S'en Va \0xBD" << endl;
			BuyTicket BT;
			vector<TicketDetails> TD = BT.getTicketList(TicketAmount, StartStation, EndStation);
			if (TD.size() == TicketAmount)//success
			{
				cout << "Confirm Information? (Y/N)" << endl;
				char Answer;
				cin >> Answer;
				if (Answer == 'Y' || Answer == 'y')
				{
					// print ticketList
					for (int i = 0; i < TicketAmount; i++)
					{
						cout << "Ticket No." << (i + 1) << " comes from:" << endl << setw(8) << left << "TNo. " << TD[i].getTreeNo() << endl << setw(8) << left << "CNo." << (TD[i].getCNo() + 1) << endl << setw(8) << left << "SeatNo." << (TD[i].getSeatNo() + 1) << endl;
						// upbit
						UpdateTree UT;
						UT.setBit(TD[i].getTreeNo(), TD[i].getCNo(), TD[i].getSeatNo(), StartStation, EndStation);
						UpdateMatrix UM;
						UM.setSoldTicket(StartStation, EndStation, TD[i].getSeatNo());
						SoldTicketLog STL;
						TicketDetails ActualTD;
						ActualTD.setSold(StartStation, EndStation - StartStation - 1, TD[i].getSeatNo());
						STL.addSoldTicket(ActualTD);
						cout << "The Ticket Sold Event No. is:" << STL.getEventNo() << ", Please Keep this Number for Refund." << endl;
						SoldTicketLogList.push_back(STL);
					}
				}
				else
				{
					cout << "Your Order Has been Cancelled." << endl;
				}
			}
			break;
		}
		case 3:
		{
			int EventNumber = 0;
			cout << "Please Enter the Event Number of the Refund Ticket:";
			cin >> EventNumber;
			if (!(EventNumber > EventNo))
			{
				SoldTicketLog CurrentSTL = SoldTicketLogList[EventNumber];
				if (EventNumber == CurrentSTL.getEventNo())
				{
					TicketDetails TD = CurrentSTL.getTicketDetails();
					cout << "Refunding..." << endl;
					cout << "Ticket came from:" << endl << setw(8) << left << "TNo. " << TD.getTreeNo() << endl << setw(8) << left << "CNo." << (TD.getCNo() + 1) << endl << setw(8) << left << "SeatNo." << (TD.getSeatNo() + 1) << endl;
					// A B C
					// A + B update A- & A+
					// A + B + C update A- & A+ C-
					// B + C update B+ C-
					// B update B+
					int RSeatNo = TD.getSeatNo();
					int RStartStation = TD.getTreeNo();//startStation
					int REndStation = RStartStation + TD.getCNo() + 1;// endStation: start + cno(from 0) + 1
					boolean StartAvailable = false;
					boolean EndAvailable = false;
					int RStartStep = 0;// tree RStartStation, C RStartCNo
					int REndStep = 0;// tree REndStation, C REndCNo
					if (RStartStation != 0 && SeatStatusMatrix[RSeatNo][RStartStation - 1] == 0)// former for sell
					{
						StartAvailable = true;
						for (int i = RStartStation - 1; i > -1; i--)
						{
							if (SeatStatusMatrix[RSeatNo][i] == 0)
							{
								RStartStep++;
							}
							else
							{
								break;// continuously
							}
						}
					}
					if (REndStation < TotalStation && SeatStatusMatrix[RSeatNo][REndStation] == 0)// latter for sell
					{
						EndAvailable = true;
						for (int i = REndStation; i < TotalStation; i++)
						{
							if (SeatStatusMatrix[RSeatNo][i] == 0)
							{
								REndStep++;
							}
							else
							{
								break;// continuously
							}
						}
					}
					// UP Tree
					cout << "RStartStation:" << RStartStation << ",REndStation:" << REndStation << ",StartAvailable:" << int(StartAvailable) << ", EndAvailable:" << int(EndAvailable) << ", RStartStep:" << RStartStep << ", REndStep:" << REndStep << endl;
					if (!StartAvailable)
					{
						if (!EndAvailable)// B update B+
						{
							UpdateTree UT;
							UT.refundBit(RStartStation, TD.getCNo(), RSeatNo);
						}
						else// B + C update B+ C-
						{
							UpdateTree UTB;
							UTB.refundBit(RStartStation, TD.getCNo() + REndStep, RSeatNo);
							UpdateTree UTC;
							UTC.setBit(REndStation, REndStep - 1, RSeatNo, REndStation, REndStation + REndStep);
						}
					}
					else
					{
						if (!EndAvailable)// A + B update A- & A+
						{
							UpdateTree UTA;
							UTA.setBit(RStartStation - RStartStep, RStartStep - 1, RSeatNo, RStartStation - RStartStep, RStartStation);
							UTA.refundBit(RStartStation - RStartStep, TD.getCNo() + RStartStep, RSeatNo);
						}
						else// A + B + C update A- & A+ C-
						{
							UpdateTree UTA;
							UTA.setBit(RStartStation - RStartStep, RStartStep - 1, RSeatNo, RStartStation - RStartStep, RStartStation);
							UTA.refundBit(RStartStation - RStartStep, TD.getCNo() + RStartStep + REndStep, RSeatNo);
							UpdateTree UTC;
							UTC.setBit(REndStation, REndStep - 1, RSeatNo, REndStation, REndStation + REndStep);
							cout << REndStation << ":" << REndStep - 1 << " clear" << endl;
						}
					}
					UpdateMatrix UM;
					UM.setRefundTicket(RStartStation, REndStation, RSeatNo);
				}
				else
				{
					cout << "Event No. Error." << endl;// should not occur
					break;
				}
			}
			break;
		}
		case 4:
		{
			Next = 0;
			cout << "BYE" << endl;
			break;
		}
		default:
			break;
		}
		return Next;
	}
};

int main()
{
	system("chcp 437");//or SetConsoleCP(437);garçon
	cout << endl;
	cout << "          WWW" << endl;
	cout << "        UUUUUUU" << endl;
	cout << "       WW     WW" << endl;
	cout << "      UU       UU" << endl;
	cout << "     WW  WWWWW  WW" << endl;
	cout << "     U     U     U" << endl;
	cout << "     WW    W    WW" << endl;
	cout << "      UU   U   UU" << endl;
	cout << "       WW  W  WW" << endl;
	cout << "          UUU" << endl;
	cout << "       WWWWWWWWW" << endl;
	cout << endl;
	cout << "Please Initialize the Total Number of Station:";
	cin >> TotalStation;
	cout << "Please Initialize the Total Number of Seat:";
	cin >> TotalSeat;
	InitialTree TicketTree;
	TicketTree.InitializeResult();
	bool Next = 1; // operation
	while (Next)
	{
		CheckInput cK;// if it is a valid request
		Next = cK.getCheckingResult();
	}
	system("pause");
	return 0;
}



