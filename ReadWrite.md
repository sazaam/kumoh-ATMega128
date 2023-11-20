


1. START 전송 : TWCR ➔ TWINT =1, TWEN =1, TWSTA =1
2. Ack 체크 : TWSR ➔ TWSR & 0xF8 = 0x08
3. TWDR에 SLA+W 세트 : TWDR ➔ TWDR = SLA + 0
4. SLA+R 전송 : TWCR ➔ TW =1, TW =1
5. Ack 체크 : TWSR & 0xF8 = 0x18
6. 데이터 수신 : TWCR ➔ DATA
7. Ack 체크 : TWSR ➔ TWSR & 0xF8 = 0x28                //  (다음 데이터의 경우 6, 7 반복)
8. STOP 전송 : TWCR ➔ TWINT = 1, TWSTO = 1, TWEN = 1



1. START 전송 : TWCR ➔ TWINT = 1, TWSTA = 1, TWEN = 1
2. Ack 체크 : TWSR ➔ TWSR & 0xF8 = 0x08
3. TWDR에 SLA+R 세트 : TWDR ➔ TWDR = SLA + 1
4. SLA+R 전송 : TWCR ➔ TWINT = 1, TWEN = 1
5. Ack 체크 : TWSR & 0xF8 = 0x40
6. 데이터 수신 : DATA (Sl) 
                TWCR ➔ TWINT = 1, TWEA = 1, TWEN = 1
7. Ack 체크 : TWSR ➔ TWSR & 0xF8 = 0x50
(다음 데이터의 경우 6, 7 반복)
8. STOP 전송 : TWCR ➔ TWINT = 1, TWSTO = 1, TWEN = 1
