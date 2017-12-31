#ifndef ADE7758_h
#define ADE7758_h
 
#define AWATTHR   0x01
#define BWATTHR   0x02
#define CWATTHR   0x03
#define AVARHR    0x04
#define BVARHR    0x05
#define CVARHR    0x06
#define AVAHR     0x07
#define BVAHR     0x08
#define CVAHR     0x09
#define AIRMS     0x0A
#define BIRMS     0x0B
#define CIRMS     0x0C
#define AVRMS     0x0D
#define BVRMS     0x0E
#define CVRMS     0x0F
#define FREQ      0x10
#define TEMP      0x11
#define WFORM     0x12
#define OPMODE    0x13
#define MMODE     0x14
#define WAVMODE   0x15
#define COMPMODE  0x16
#define LCYCMODE  0x17
#define MASK      0x18
#define STATUS    0x19
#define RSTATUS   0x1A
#define ZXTOUT    0x1B
#define LINECYC   0x1C
#define SAGCYC    0x1D
#define SAGLVL    0x1E
#define VPINTLVL  0x1F
#define IPINTLVL  0x20
#define VPEAK     0x21
#define IPEAK     0x22
#define GAIN      0x23
#define AVRMSGAIN 0x24
#define BVRMSGAIN 0x25
#define CVRMSGAIN 0x26
#define AIGAIN    0x27
#define BIGAIN    0x28
#define CIGAIN    0x29
#define AWG       0x2A
#define BWG       0x2B
#define CWG       0x2C
#define AVARG     0x2D
#define BVARG     0x2E
#define CVARG     0x2F
#define AVAG      0x30
#define BVAG      0x31
#define CVAG      0x32
#define AVRMSOS   0x33
#define BVRMSOS   0x34
#define CVRMSOS   0x35
#define AIRMSOS   0x36
#define BIRMSOS   0x37
#define CIRMSOS   0x38
#define AWAITOS   0x39
#define BWAITOS   0x3A
#define CWAITOS   0x3B
#define AVAROS    0x3C
#define BVAROS    0x3D
#define CVAROS    0x3E
#define APHCAL    0x3F
#define BPHCAL    0x40
#define CPHCAL    0x41
#define WDIV      0x42
#define VADIV     0x44
#define VARDIV    0x43
#define APCFNUM   0x45
#define APCFDEN   0x46
#define VARCFNUM  0x47
#define VARCFDEN  0x48
#define CHKSUM    0x7E
#define VERSION   0x7F 


#define CS 10                // Chip Select ADE7753 - Digital output pin nbr on Olimex Energy Shield 
#define WRITE 0x80           // WRITE bit BT7 to write to registers
#define CLKIN 10000000    



 
class ADE7758{
 
 
public:
 
        //Voltage phase A
          void setAVRMSOS (int avos);          //write something into AVRMS register
          int readAVRMSOS (void);                    //read something from AVRMS register
          long readAVRMS(void);
        // For Current A   
          void setAIRMSOS (int avos);          //write something into AIRMS register
          int readAIRMSOS (void);                    //read something from AIRMS register
          long readAIRMS(void);

          //Voltage phase B
          void setBVRMSOS (int avos);          //write something into BVRMS register
          int readBVRMSOS (void);                    //read something from BVRMS register
          long readBVRMS(void);
        // For Current B   
          void setBIRMSOS (int avos);          //write something into BIRMS register
          int readBIRMSOS (void);                    //read something from BIRMS register
          long readBIRMS(void);
          
        //Voltage phase C
          void setCVRMSOS (int avos);          //write something into CVRMS register
          int readCVRMSOS (void);                    //read something from CVRMS register
          long readCVRMS(void);
        // For Current B   
          void setCIRMSOS (int avos);          //write something into CIRMS register
          int readCIRMSOS (void);                    //read something from CIRMS register
          long readCIRMS(void);
          
      // For power
          void setAWAITOS (int avos);          //write something into AWATTHR register
          int readAWAITOS (void);                    //read something from AWATTHR register
          long readAWATTHR(void);

          
          void setSPI(void);
          void closeSPI(void);
          void enableChip(void);
          void disableChip(void);
          unsigned char read8(char reg);

          void write16(char reg, unsigned int data);
          void write8(char reg, unsigned int data);
          int lineFreq(char phase);
          unsigned int read16(char reg);

          unsigned long read24(char reg);
 
 
};
 
 
#endif
