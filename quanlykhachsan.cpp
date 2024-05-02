#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;
const int maxRooms = 100;
const string fileName = "data.txt";
struct Date {
    int day;
    int month;
    int year;
    bool isEmpty;
    int dayInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    string toString() {
    	if (isEmpty){
    		return "";
		}
        return to_string(day) + "/" + to_string(month) + "/" + to_string(year);
    }
    //Lay ra so ngay tu ngay 0/0/0
    string getDays(int numberDays) {
        int totalDays = toDays() + numberDays;
        Date newDate; 
        newDate.year = totalDays / 365;
        totalDays %= 365;
        int i;
        for (i = 0; i < 12; i++) {
            if (totalDays <= dayInMonth[i]) break;
            totalDays -= dayInMonth[i];
        }
        newDate.month = i + 1;
        newDate.day = totalDays;
        return newDate.toString();
    }
    int toDays() {
    	if (isEmpty){
    		return 0;
		}
        int totalDays = year * 365 + day;
        for (int i = 0; i < month - 1; i++) {
            totalDays += dayInMonth[i];
        }
        bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (month > 2 && isLeapYear){
            totalDays++;
        }
        return totalDays;
    }
    bool validDate() {
        if (month < 1 || month > 12 || day < 1 || day > dayInMonth[month - 1]) {
            return false;
        }
        return true;
    }
    void toDate(string dateInput){
    	isEmpty = false;
        string temp;
        bool validInput = false;
        while (!validInput) {
            int index = 0;
            temp = "";
            for (int i = 0; i <= dateInput.length(); ++i) {
                if (i == dateInput.length() || dateInput[i] == '/') {
                    if (index == 0) day = stoi(temp);
                    else if (index == 1) month = stoi(temp);
                    else if (index == 2) year = stoi(temp);
                    temp = "";
                    index++;
                } else {
                    temp += dateInput[i];
                }
            }
            if (validDate()) {
                validInput = true;
            } else {
                cout << "Ngay khong hop le. Vui long nhap lai." << endl;
            }
        }
	}
	void input(){
    	isEmpty = false;
        string dateInput, temp;
        bool validInput = false;
        while (!validInput) {
        	cout << "Nhap ngay dd/mm/yyyy: ";
        	cin >> dateInput;
            int index = 0;
            temp = "";
            for (int i = 0; i <= dateInput.length(); ++i) {
                if (i == dateInput.length() || dateInput[i] == '/') {
                    if (index == 0) day = stoi(temp);
                    else if (index == 1) month = stoi(temp);
                    else if (index == 2) year = stoi(temp);
                    temp = "";
                    index++;
                } else {
                    temp += dateInput[i];
                }
            }
            if (validDate()) {
                validInput = true;
            } else {
                cout << "Ngay khong hop le. Vui long nhap lai." << endl;
            }
        }
    }
};
struct Room {
    string roomCode;
    char status; 
    double price;
    double discount;
    Date startDate;
    Date endDate;
    long totalDays;
    double totalPrice;
    bool validDate(){
    	return endDate.toDays()>=startDate.toDays();
	}
    bool isEmpty(){
        return roomCode.empty();
    }
    int getTotalDay(){
    	if (!endDate.isEmpty && !startDate.isEmpty){
    		return endDate.toDays() - startDate.toDays() + 1;
		};
		return 0;
    }
};
struct Hotel {
    string name;
    int totalRooms;
    Room rooms[maxRooms];
    string address;
};
void readData(Hotel &hotel){
	fstream f(fileName,ios::in);
	f >> hotel.name;
	f >> hotel.address;
	f >> hotel.totalRooms;
	for (int i=0;i<hotel.totalRooms;i++){
		Room room;
		f >> room.roomCode >> room.status >> room.price;
		if (room.status == 'G'){
			f >> room.discount;
		} else {
			room.discount = 0;
		}
		if (room.status != 'K'){
			string startDate,endDate; 
			f >> startDate >> endDate;
			room.startDate.toDate(startDate);
			room.endDate.toDate(endDate);
			room.totalDays = room.getTotalDay();
			room.totalPrice = room.price * room.totalDays * (100 - room.discount) / 100;
		} else {
			room.totalDays = 0;
			room.totalPrice = 0;
			room.startDate.isEmpty  = true;
			room.endDate.isEmpty = true;
		}
		hotel.rooms[i] = room;
	}
	f.close();
}
void saveData(Hotel &hotel) {
	ofstream f(fileName, ios_base::trunc);
	f << hotel.name << " " << hotel.address << " " << hotel.totalRooms << endl;
    if (f.is_open()) {
    	for(int i=0;i<hotel.totalRooms;i++){
	    	f << hotel.rooms[i].roomCode << " " << hotel.rooms[i].status << " " << hotel.rooms[i].price << " "; 
	        if (hotel.rooms[i].status == 'G') {
	            f << hotel.rooms[i].discount << " ";
	        }
	        f << hotel.rooms[i].startDate.toString() << " " << hotel.rooms[i].endDate.toString() << "\n";
		}
        f.close();
    } else {
        std::cerr << "Không thể mở tệp tin!\n";
    }
}

void addHotel(Hotel &hotel);
void addRoom(Hotel &hotel);
void displayRoom(Room &room);
void displayRooms(Hotel &hotel);
void searchRoom(Hotel &hotel, string& roomCode);
void statistics(Hotel &hotel);

string toString(double n){
	string s = to_string(n);
	return s.erase(s.size() - 4);
}
void addRoom(Hotel &hotel) {
	if (hotel.name.empty()){
		cout << "Vui long nhap thong tin khach san\n";
		addHotel(hotel);
	} else {
	    if (hotel.totalRooms >= maxRooms) {
	        cout << "Khong the them phong. Da dat toi da so phong." << endl;
	        return;
	    }
	    Room room;
	    cout << "****************************" << endl;
	    cout << "- Nhap ma phong: ";
	    cin.ignore();
	    getline(cin,room.roomCode);
	    bool validRoomCode = false;
	    while (!validRoomCode) {
	        bool isFound = false;
	        for (int i = 0; i < hotel.totalRooms; i++) {
	            if (hotel.rooms[i].roomCode == room.roomCode) {
	                isFound = true;
	                break;
	            }
	        }
	        if (!isFound) {
	            validRoomCode = true;
	        } else {
	            cout << "Ma phong da ton tai. Nhap ma phong moi: ";
	            cin >> room.roomCode;
	        }
	    }
	
	    bool validStatus = false;
	    while (!validStatus) {
	        cout << "- Nhap trang thai phong (K,T,G): ";
	        cin >> room.status;
	        if (room.status == 'K' || room.status == 'T' || room.status == 'G') {
	            validStatus = true;
	        } else {
	            cout << "Trang thai khong hop le. Vui long nhap K, T, hoac G." << endl;
	        }
	    }
	    cout << "- Nhap gia phong: ";
		cin >> room.price;
		while (room.price < 0) {
		    cout << "Gia khong the la so am. Vui long nhap gia hop le: ";
		    cin >> room.price;
		}
		if (room.status == 'G') {
		        cout << "- Nhap % giam gia: ";
		        cin >> room.discount;
		        while (room.discount < 0 || room.discount > 100) {
		            cout << "Giam gia phai nam trong khoang tu 0 den 100. Vui long nhap giam gia hop le: ";
		            cin >> room.discount;
		        }
		    } else {
		        room.discount = 0;
		}
	    if (room.status == 'G' || room.status == 'T'){
		    do {
		        cout << "- Ngay bat dau\n";
		        room.startDate.input();
		        cout << "- Ngay ket thuc\n";
		        room.endDate.input();
		        if (!room.validDate()) {
		            cout << "Ngay bat dau phai truoc ngay ket thuc. Vui long nhap lai.\n";
		        }else {
		        	room.totalDays = room.getTotalDay();
				}
		    } while (!room.validDate());
		
		    room.totalPrice = room.price * room.totalDays * (100 - room.discount) / 100;
		} else {
			room.startDate.isEmpty  = true;
			room.endDate.isEmpty = true;
			room.totalDays = 0;
			room.totalPrice = 0;
		}
		for (int index=0;index<hotel.totalRooms;index++){
	    	if (hotel.rooms[index].isEmpty()){
	    		hotel.rooms[index] = room;
	    		saveData(hotel);
	    		break;
			}
		}
	}
}

void displayRooms(Hotel &hotel){
	cout << "+ ----------+--------+--------+-----------+-------------+-----------+-------------+-------------+-------------+------------- +" << endl;
	cout << "| " << setw(10) << left << "Ma Phong" << " | " << setw(9) << left << "Trang Thai" << " | " << setw(16) << left << "Gia" << " | " << setw(14) << left << "Giam Gia" << " | " << setw(13) << left << "Ngay Bat Dau" << " | " << setw(10) << left << "Ngay Ket Thuc" << " | " << setw(15) << left << "Tong so ngay" << " | " << setw(14) << left << "Tong Gia" << " |" << endl;
	for (int i = 0; i < hotel.totalRooms; i++) {
	    Room room = hotel.rooms[i];
	    if (!room.isEmpty()) {
	        cout << "+ ----------+--------+--------+-----------+-------------+-----------+-------------+-------------+-------------+------------- +" << endl;
	        string price = toString(room.price) + "VND";
	        string discount = toString(room.discount) + "%";
	        string totalPrice = toString(room.totalPrice) + "VND";
	        cout << "| " << setw(10) << left << room.roomCode << " | " << setw(9) << left << room.status << " | " << setw(16) << left << price << " | " << setw(14) << left << discount << " | " << setw(13) << left << room.startDate.toString() << " | " << setw(10) << left << room.endDate.toString() << " | " << setw(15) << left << room.totalDays << " | " << setw(14) << left << totalPrice << " |" << endl;
	    } else {
	        break;
	    }
	}
	cout << "+ ----------+--------+--------+-----------+-------------+-----------+-------------+-------------+-------------+------------- +" << endl;
}
void searchRoom(Hotel &hotel,string &roomCode) {
    bool found = false;
    for (int i = 0; i < hotel.totalRooms; ++i) {
        if (hotel.rooms[i].roomCode == roomCode) {
            found = true;
            cout << "************************************" << endl;
            cout << "* Ma Phong: " << hotel.rooms[i].roomCode << endl;
            cout << "* Trang Thai: " << hotel.rooms[i].status  << endl;
            cout << "* Gia: " << hotel.rooms[i].price  << endl;
            cout << "* Giam Gia: " << hotel.rooms[i].discount << "%" << endl;
            break;
        }
    }
    if (!found) {
        cout << "Khong tim thay phong." << endl;
    }
}
double displayOccupancyRate(Hotel hotel, int day) {
    int occupiedRooms = 0;
    for (int i = 0; i < hotel.totalRooms; ++i) {
        Room room = hotel.rooms[i];
        if (!room.isEmpty() && room.status != 'K' && room.startDate.toDays() <= day && room.endDate.toDays() >= day) {
            occupiedRooms++;
        }
    }
    return (double)occupiedRooms / hotel.totalRooms * 100;
}
void displayDailyRevenue(Hotel hotel, Date startDate, Date endDate) {
    int numberDays = endDate.toDays() - startDate.toDays();
    double dailyRevenue[numberDays]; 
    bool foundValidRoom = false;
    for (int i = 0; i < numberDays; ++i) {
        dailyRevenue[i] = 0.0; 
    }
    for (int i = 0; i < hotel.totalRooms; i++) {
        Room room = hotel.rooms[i];
        if (!room.isEmpty()) {
            if (room.startDate.toDays() <= endDate.toDays() && room.endDate.toDays() >= startDate.toDays()) {
                foundValidRoom = true;
                Date actualStartDay;
                if (room.startDate.toDays() > startDate.toDays()) {
                    actualStartDay = room.startDate;
                } else {
                    actualStartDay = startDate;
                }
                Date actualEndDay;
                if (room.endDate.toDays() < endDate.toDays()) {
                    actualEndDay = room.endDate;
                } else {
                    actualEndDay = endDate;
                }
                for (int day = actualStartDay.toDays(); day <= actualEndDay.toDays(); ++day) {
                    dailyRevenue[day - startDate.toDays()] += room.price; 
                }
            }
        }
    }

    if (!foundValidRoom) {
        cout << "Khong co doanh thu trong trong khoang thoi gian da chon." << endl;
    } else {
        cout << "Doanh Thu Hang Ngay tu " << startDate.toString() << " den " << endDate.toString() << ":\n";
        cout << "+--------------+---------------++-----------------------+\n";
        cout << "|   Ngay       |   Doanh Thu   |   Ti le phong cho thue  \n";
        cout << "+--------------+---------------++-----------------------+\n";
        for (int day = 0; day < numberDays; ++day) {
        	string occupancyRate = toString(displayOccupancyRate(hotel,startDate.toDays()+day)) + "%";
            cout << "| " << setw(1)  << startDate.getDays(day) << "     |  " << setw(8) << toString(dailyRevenue[day]) << "VND  |" << setw(5) << occupancyRate << endl;
        }
        cout << "+--------------+---------------+-----------------------+\n";
        double totalPrice = 0;
        for (int i = 0; i < numberDays; ++i) {
	        totalPrice += dailyRevenue[i];
	    }
        cout << "Tong doanh thu: " << toString(totalPrice) << "VND\n";
    }
}
void displayHotelInfo(Hotel &hotel) {
	if (hotel.name.empty()){
		cout << "Vui long nhap thong tin khach san\n";
		addHotel(hotel);
	}
	cout << "************************************\n";
    cout << "- Thong Tin Khach San:" << endl;
    cout << "* Ten: " << hotel.name << endl;
    cout << "* Dia Chi: " << hotel.address << endl;
    cout << "* Tong So Phong: " << hotel.totalRooms << endl;
}
void displayRoomsByStatus(Hotel &hotel, char status) {
    if (status=='G'){
        cout << "+ ----------+--------+--------+-----------+-------------+-----------+-------------+-------------+-------------+------------- +" << endl;
	} else if (status=='T'){
		cout << "+ ----------+--------+--------+-----------+-------------+-----------+-------------+-------------+------------- +" << endl;
	} else {
        cout << "+ ----------+--------+--------+----------- +" << endl;
	}
    cout << "| " << setw(10) << left << "Ma Phong" << " | " << setw(9) << left << "Trang Thai" << " | " << setw(16) << left << "Gia" << " | ";
    if (status =='G'){
    	cout << setw(14) << left << "Giam Gia" << " | ";
	}
    if (status != 'K') {
        cout << setw(13) << left << "Ngay Bat Dau" << " | " << setw(10) << left << "Ngay Ket Thuc" << " | " << setw(15) << left << "Tong so ngay" << " | " << setw(14) << left << "Tong Gia"  << " |" << endl;
    } else {
        cout << endl;
    }
    for (int i = 0; i < hotel.totalRooms; i++) {
        Room room = hotel.rooms[i];
        if (!room.isEmpty() && room.status == status) {
        	    if (status=='G'){
			        cout << "+ ----------+--------+--------+-----------+-------------+-----------+-------------+-------------+-------------+------------- +" << endl;
				} else if (status=='T'){
					cout << "+ ----------+--------+--------+-----------+-------------+-----------+-------------+-------------+------------- +" << endl;
				} else {
			        cout << "+ ----------+--------+--------+----------- +" << endl;
				}
			string price = toString(room.price) + "VND";
            string discount = toString(room.discount) + "%";
            cout << "| " << setw(10) << left << room.roomCode << " | " << setw(9) << left << room.status << " | " << setw(16) << left << price << " | ";
            if (status=='G'){
            	cout << setw(14) << left << discount << " | ";
			}
            if (status != 'K') {
                string startDate = room.startDate.toString();
                string endDate = room.endDate.toString();
                string totalDays = toString(room.totalDays);
                string totalPrice = toString(room.totalPrice) + "VND";
                cout << setw(13) << left << startDate << " | " << setw(10) << left << endDate << " | " << setw(15) << left << totalDays << " | " << setw(14) << left << totalPrice << " |" << endl;
            } else {
                cout << endl;
            }
        }
    }
    if (status=='G'){
        cout << "+ ----------+--------+--------+-----------+-------------+-----------+-------------+-------------+-------------+------------- +" << endl;
	} else if (status=='T'){
		cout << "+ ----------+--------+--------+-----------+-------------+-----------+-------------+-------------+------------- +" << endl;
	} else {
        cout << "+ ----------+--------+--------+----------- +" << endl;
	}
}
void displayRoomsByStatus(Hotel &hotel) {
    cout << "Cac Phong co Giam Gia (G):" << endl;
    displayRoomsByStatus(hotel, 'G');
    cout << "\nCac Phong Chua Cho Thue (K):" << endl;
    displayRoomsByStatus(hotel, 'K');
    cout << "\nCac Phong Binh Thuong (T):" << endl;
    displayRoomsByStatus(hotel, 'T');
}
void displayTotalRevenue(Hotel &hotel) {
    double totalRevenue = 0.0;
    for (int i = 0; i < hotel.totalRooms; ++i) {
        Room room = hotel.rooms[i];
        if (!room.isEmpty()) {
            totalRevenue += room.totalPrice;
        }
    }
    cout << "Tong doanh thu cua khach san: " << toString(totalRevenue) << "VND" << endl;
}
void statistics(Hotel &hotel) {
	int choice;
	bool display = true;
    while(display){
	    cout << "*************************************\n";
	    cout << "*          MENU THONG KE            *\n";
	    cout << "*************************************\n";
	    cout << "* 0. Doanh Thu                      *\n";
	    cout << "* 1. Doanh Thu Theo Ngay            *\n";
	    cout << "* 2. Danh Sach Phong                *\n";
	    cout << "* 3. Thoat                          *\n";
	    cout << "*************************************\n";
	    cout << "Nhap lua chon cua ban: ";
	    cin >> choice;
 		switch (choice) {
 			case 0:
 				displayTotalRevenue(hotel);
 				break;
	        case 1:
	        {
	            Date startDate; 
				Date endDate;
				bool validDate = true;
	            do {
			        cout << "- Ngay bat dau\n";
			        startDate.input();
			        cout << "- Ngay ket thuc\n";
			        endDate.input();
			        validDate = startDate.toDays()>endDate.toDays();
			        if (validDate) {
			            cout << "Ngay bat dau phai truoc ngay ket thuc. Vui long nhap lai.\n";
			        }
			    } while (validDate);
	            displayDailyRevenue(hotel, startDate, endDate);
	            break;
	        }
	        case 2:
	        	displayRoomsByStatus(hotel);
	        	break;
	        case 3:
	        	display = false;
	        	break;
	        default:
	            cout << "Lua chon khong hop le. Vui long thu lai." << endl;
		}
	}
}
void addHotel(Hotel &hotel){
	cout << "+ Nhap ten Khach San: ";
	cin.ignore();
    getline(cin, hotel.name);
    cout << "+ Nhap dia chi: ";
    getline(cin, hotel.address);
    do {
        cout << "+ Nhap tong so phong: ";
        cin >> hotel.totalRooms;
        if (hotel.totalRooms > maxRooms) {
            cout << "+ So luong phong vuot qua gioi han toi da (" << maxRooms << "). Vui long nhap lai." << endl;
        }
    } while (hotel.totalRooms > maxRooms);
    for (int i = 0; i < hotel.totalRooms; i++) {
        cout << "+ Nhap thong tin phong " << i + 1 << ":\n";
        addRoom(hotel);
    }
}
void hotelManagementMenu(Hotel &hotel){
	bool display = true;
	while (display){
		int choice;
		cout << "************************************\n";
		cout << " CHAO MUNG DEN VOI KHACH SAN " << hotel.name << endl;
		cout << "* 0. Thong Tin Khach San          *\n";
	    cout << "* 1. Them Phong                   *\n";
	    cout << "* 2. Hien Thi Phong               *\n";
	    cout << "* 3. Tim Kiem Phong               *\n";
	    cout << "* 4. Thong Ke                     *\n";
	    cout << "* 5. Thoat                        *\n";
	    cout << "************************************\n";
	    cout << "Nhap lua chon cua ban: ";
	    cin >> choice;
	        switch (choice) {
	        	case 0:
	        		displayHotelInfo(hotel);
	        		break;
	            case 1:
	            	if (hotel.totalRooms >= maxRooms){
	            		cout << "Khong the them phong";
					} else {
						hotel.totalRooms += 1;
	                	addRoom(hotel);
					}
					cout << "Them phong thanh cong!\n";
					break;
	            case 2:
	            {
	            	displayRooms(hotel);
	                break;
	        	}
				case 3: {
	                string roomCode;
	                cout << "Nhap ma phong:  ";
	                cin >> roomCode;
	                searchRoom(hotel, roomCode);
	                break;
	       		}
	            case 4:
	                statistics(hotel);
	                break;
	            case 5:
	                cout << "Dang thoat chuong trinh." << endl;
	                display = false;
	                break;
	            default:
	                cout << "Lua chon khong hop le. Vui long thu lai." << endl;
	        }
	    }
};
int main() {
	Hotel hotel;
	readData(hotel);
	hotelManagementMenu(hotel);
	return 0;
}
