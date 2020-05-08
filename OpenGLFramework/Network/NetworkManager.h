#pragma once

#define PORT_UDP			3467	//TODO: umiescic w opcjach
#define PORT_TCP			3468	//TODO: umiescic w opcjach
#define DATA_SIZE			1024
#define PROTOCOL_HEADER		"oGLf"	//size = HEADER_SIZE
#define PROTOCOL_END_DATA	"EndD"	//size = HEADER_SIZE
#define HEADER_SIZE			4		//Wielko�� PROTOCOL_HEADER i PROTOCOL_END_DATA
#define HOSTNAME_SIZE		64		//64 bo tyle mozna podac w systemie

/// Typ wyliczeniowy rodzaj�w pakiet�w.
enum EPacketType
{
	/// Pakiet wysy�any na chat informuj�cy o do��czeniu u�ytkownika.
	/** SPacketUDP::cData zawiera pe�ny komuniat, np: "[12:00] Pseudonim joined".	*/
	EPacketTypeJoinUser,

	/// Pakiet wiadomo�ci wys�anej na chat.
	/** SPacketUDP::cData zawiera pe�ny komuniat, np: "[12:00] <Pseudonim> Tre�� wiado�ci".	*/
	EPacketTypeChatMsg,

	/// Pakiet wysy�any co 5 sekund informuj�cy wszystkich o stworzeniu gry.
	/** SPacketUDP::cData zawiera struktur� SPacketCreateEcho. */
	EPacketTypeCreateEcho,

	/// Pakiet informauj�cy o do��czeniu si� gracza do gry.
	/** SPacketUDP::cData zawiera struktur� SPacketJoin. */
	EPacketTypeJoin,

	/// Pakiet informuj�cy do��czaj�cego, �e za�o�yciel gry ju� nie oczekuje na po��czenie.
	/** SPacketUDP::cData puste. */
	EPacketTypeCreatorNotWait,

	/// Pekiet informuj�cy do��czaj�cego o akceptacji po��czenia przez za�o�yciela gry.
	/** SPacketUDP::cData puste. */
	EPacketTypeAcceptJoin,

	/// Pakiet informuj�cy do��czaj�cego o odrzuceniu po��czenia przez za�o�yciela gry.
	/** SPacketUDP::cData zawiera pseudonim gracza tworz�cego gr�. */
	EPacketTypeRejectJoin,

	/// Pakiet informuj�cy za�o�yciela gry o zawi�zaniu transakcji po��czenia.
	/** SPacketUDP::cData puste. */
	EPacketTypeTransactionOfConnectDone,

	/// Pakiet informuj�cy za�o�yciela gry o rozpocz�ciu gry.
	/** SPacketUDP::cData puste. */
	EPacketTypeStartMultiGame
};

struct SPacketCreateEcho
{
	char cCreatorNickname[ MAX_MULTIPLAYER_NICKNAME ];
	char cCreatorHostname[ HOSTNAME_SIZE ];
	unsigned char bOptionCollision;
	unsigned char bOptionSameShips;
	int iIndexShip; //indeks statku jakim bedzie gral "tworca gry"
	unsigned int uiLastRef;
};

struct SPacketJoin
{
	char cNickname[ MAX_MULTIPLAYER_NICKNAME ];
	char cHostname[ HOSTNAME_SIZE ];
	int iIndexShip; //indeks statku jakim bedzie gral "dolaczajacy"
};

/// Uniwersalna struktura, kt�ra zawsze jest wysy�ana protoko�em UDP.
struct SPacketUDP
{
	char cHeader[ HEADER_SIZE ];			///< Nag��wek identyfikacyjny. Zawsze PROTOCOL_HEADER.
	int iPacketType;						///< Typ pakietu.
	unsigned int uiIPSender;				///< Adres IP nadawcy pakietu.
	unsigned int uiIPReceiver;				///< Adres IP odbiorcy pakietu.

	int iDataLength;						///< Wielko�� przesy�anych danych.
	unsigned char cData[DATA_SIZE];			///< Przesy�ane dane. Musi ko�czy� si� PROTOCOL_END_DATA.
};

#define PACKET_SIZE sizeof( SPacketUDP )

/// Struktura przekazywana do w�tku wysy�aj�cego i odbieraj�cego transfer TCP.
struct STransferTCPData
{
	bool bSender;							///< Flaga informuj�ca czy jestem nadawc� czy odbiorc� danych.
	int iPortNumber;						///< Numer portu, na kt�rym nast�pi po��czenie.
	char *pData;							///< Wska�nik na dane do wys�ania/odebrania przez transfer TCP.
	int iDataLength;						///< Wielko�� (w bajtach) danych zawartych w pData;
	
	/// Wska�nik na funkcj�, kt�ra zostanie wywo�ana po zako�czeniu w�tku wysy�aj�cego/odbieraj�cego.
	/**  Funkcja ta musi przyjmowa� dwa parametry. Pierwszy typu \p int b�d�cy rezultatem dzia�ania w�tku. Je�eli
	funkcja otrzyma warto�� 0 to praca w�tku zako�czy�a si� niepowodzeniem. Je�eli 1 to praca w�tku zako�czy�a
	si� pomy�lnie. Drugi parametr to wska�nik na w�as� struktur� STransferTCPData.
	Zamiast funkcji mo�emy tak�e poda� tutaj warto�� \p NULL. */
	void( *Callback )( int, STransferTCPData* );
};

class CNetworkManager
{
public:
	CNetworkManager();
	~CNetworkManager();

	GLboolean CreateUDPSocket();
	GLboolean StartRecvUDPThread();
	GLboolean StopRecvUDPThread();
	GLboolean SendUDP( int iPacketType, const void *pData, int iDataLength, const char *cIPReceiver );
	GLboolean SendUDP( int iPacketType, const void *pData, int iDataLength, unsigned int uiIPReceiver = INADDR_BROADCAST );

	SOCKET GetSocketUDP()		{ return m_uiSocketUDP; }
	SOCKADDR_IN* GetRecvAddr()	{ return &m_sRecvAddr; }

	char* GetHostName()			{ return m_cMyHostName; }
	char* GetMyIP()				{ return m_pMyIP; }

	/// Metoda konwertuj�ca adres IP z liczby na tekst w UNICODE lub ASCII.
	/** \param[in] uiIP Liczba reprezentuj�ca adres IP. 
	\return �acuch znak�w adresu IP otrzymanego z liczby, np: _T("192.168.0.1"). */
	LPTSTR IPntoaw( unsigned int uiIP );

	///////////////////// TCP //////////////////////////////////////////////

	GLboolean StartSendTCPTransfer( STransferTCPData *in_sTransferTCPData );
	GLboolean StopSendTCPTransfer();
	GLboolean StartRecvTCPTransfer( STransferTCPData *out_sTransferTCPData );
	GLboolean StopRecvTCPTransfer();

private:
	GLboolean m_bInitSockets;
	GLvoid CheckMyIPAndHost();
	char m_cMyHostName[ 256 ];
	char *m_pMyIP;

	/// Uchwyt gniazda protoko�u UDP.
	SOCKET m_uiSocketUDP;

	SOCKADDR_IN	m_sRecvAddr;	//UDP odbieraj�cy
	SOCKADDR_IN	m_sSendAddr;	//UDP wysy�aj�cy

	/// Uchwyt do w�tku odbieraj�cego pakiety UDP.
	HANDLE m_hRecvUDPThread;

	/// Uchwyt do w�tku transmituj�cego dane na protokole TCP. 
	HANDLE m_hSendTCPTransfer;

	/// Uchwyt do w�tku odbieraj�cego dane z transmisji na protokole TCP. 
	HANDLE m_hRecvTCPTransfer;

	/// Metoda konwertuj�ca adres IP z liczby na tekst w ASCII.
	/** \param[in] uiIP Liczba reprezentuj�ca adres IP. 
	\return �a�cuch znak�w adresu IP otrzymanego z liczby, np: "192.168.0.1". */
	char* IPntoa( unsigned int uiIP );

	/// Metoda konwertuj�ca adres IP z liczby na tekst w UNICODE.
	/** \param[in] uiIP Liczba reprezentuj�ca adres IP. 
	\return �a�cuch znak�w adresu IP otrzymanego z liczby, np: _T("192.168.0.1"). */
	wchar_t* IPntow( unsigned int uiIP );
};
