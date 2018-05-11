#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "protocol.h"
#include "datalink.h"

#define DATA_TIMER  5000 //Data֡��ʱʱ��
#define ACK_TIMER 280 //Ack֡��ʱʱ��

#define MAX_SEQ 31 //֡����ſռ䣬Ӧ����2^n-1
#define NR_BUFS 16 //���ʹ��ڡ����մ��ڴ�С��NR_BUFS=(MAX_SEQ+1)/2
#define inc(k) if(k < MAX_SEQ)k++;else k=0 //����k+1  

typedef  unsigned char seq_nr;
struct FRAME { //֡������
	seq_nr kind; //֡�����࣬��ΪAck, Nak �� Data ����
	seq_nr ack; ////�Ӵ�ȷ��ack
	seq_nr seq; //��֡�����
	seq_nr info[PKT_LEN]; //����,�洢��С256
};

static unsigned char next_frame_to_send = 0, frame_expected = 0, ack_expected = 0;
//next_frame_to_send:��Ҫ���͵�֡��ţ����ͷ����ڵ��Ͻ磻frame_expected:�����յ���֡��ţ����շ����ڵ��½磻ack_expected:�����յ���ack֡��ţ����ͷ����ڵ��½�
static unsigned char out_buffer[NR_BUFS][PKT_LEN], in_buffer[NR_BUFS][PKT_LEN], nbuffered;
//��Ӧ������桢���뻺�棬�Լ�Ŀǰ�������ĸ���
static unsigned char too_far = NR_BUFS; //���շ����ڵ��½�
static int phy_ready = 0; //�������Ƿ�ready
bool arrived[NR_BUFS]; //���շ����뻺��Ĵ��ڣ� 
bool no_nak = true; //�Ƿ�����NAK��true�����ظ�����

static bool between(int a, int b, int c){ //���b��a��c��ɵĴ���֮�䣬�򷵻�true�����򷵻�false
	return((a <= b) && (b < c)) || ((c < a) && (a <= b)) || ((b < c) && (c <a));
}

static void add_crc32(unsigned char *frame, int len){ 
	//��frame��CRCУ��󸽼����һ���͵�������
	*(unsigned int *)(frame + len) = crc32(frame, len);
	send_frame(frame, len + 4);// ���͵������㣬4�ֽڼ�� 
	phy_ready = 0; //�����ݷ��͵�������󣬽���������Ϊæµ״̬
}

static void send_data_frame(unsigned char frame_nr){//��������֡ 
	//�������������frame�����add_crc32���͵������㣬ͬʱ��ʼ��ʱdata_timer
	struct FRAME s;

	s.kind = FRAME_DATA;
	s.seq = frame_nr;
	s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);
	memcpy(s.info, out_buffer[frame_nr % NR_BUFS], PKT_LEN); //��out_buffer��Ļ��渴�Ƶ�֡frame��info����
	
	dbg_frame("Send DATA %d %d, ID %d\n", s.seq, s.ack, *(short *)s.info);
	
	add_crc32((unsigned char *)&s, 3 + PKT_LEN); //������õ�֡���͵������㣬3�ֽڿ�����Ϣ 
	
	start_timer(frame_nr % NR_BUFS, DATA_TIMER);
	stop_ack_timer();
}

static void send_ack_frame(void){
	//��ack��װ��֡���͵�������
	struct FRAME s;

	s.kind = FRAME_ACK;
	s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);

	dbg_frame("Send ACK  %d\n", s.ack);

	add_crc32((unsigned char *)&s, 2);
	stop_ack_timer();
}

static void send_nak_frame(void){
	//��ack��װ��֡���͵�������
	struct FRAME s;

	s.kind = FRAME_NAK;
	s.ack = (frame_expected + MAX_SEQ) % (MAX_SEQ + 1);

	no_nak = false;

	dbg_frame("Send NAK with ACK %d\n", s.ack);

	add_crc32((unsigned char *)&s, 2);//������õ�֡���͵�������
	stop_ack_timer();
}

int main(int argc, char **argv)
{
	//��ʼ��
	nbuffered = 0; //��ʼû�����֡������
	int i;//arrived[]��into buffer[]����ţ� 
	for (i = 0; i < NR_BUFS; i++) //���մ��ڳ�ʼ�� 
		arrived[i] = false;
	int event, oldest_frame;//�¼������ʶû��ȷ�ϵ�֡��� 
	struct FRAME f;
	int len = 0; //�յ�data�ĳ���

	protocol_init(argc, argv); //Э���ʼ��

	disable_network_layer();

	for (;;)
	{
		event = wait_for_event(&oldest_frame);

		switch (event) //�Բ�ͬ�¼����в�ͬ����
		{
			case NETWORK_LAYER_READY: //�������readyʱ
				get_packet(out_buffer[next_frame_to_send % NR_BUFS]); //��������ȡһ֡�������������
				nbuffered++; //�������+1
				send_data_frame(next_frame_to_send);//���͸�֡
				inc(next_frame_to_send); //�������ͷ���������+1
				break;

			case PHYSICAL_LAYER_READY: //��������readyʱ����phy_ready��Ϊ1�Ա�֮��enable�����
				phy_ready = 1;
				break;

			case FRAME_RECEIVED: //���������յ�һ֡ʱ
				len = recv_frame((unsigned char *)&f, sizeof f); //���������ȡһ֡
				if (len < 5 || crc32((unsigned char *)&f, len) != 0) { //������ջ�֡��CRCУ��ʧ�ܣ��򷵻�nak֡
					dbg_event("**** Receiver Error, Bad CRC Checksum\n");
					if (no_nak)
						send_nak_frame();
					break;
				}
				if (f.kind == FRAME_ACK) //�����ack֡�Ļ�����������֡������ack֡�����ͳһ����
					dbg_frame("Recv ACK  %d\n", f.ack);

				if (f.kind == FRAME_DATA) //���������֡
				{
					if ((f.seq != frame_expected) && no_nak) //����յ����ǲ���Ҫ��֡�򷵻�nak
						send_nak_frame();
					else
						start_ack_timer(ACK_TIMER);
					if (between(frame_expected, f.seq, too_far) && (arrived[f.seq%NR_BUFS] == false))
					//����յ���֡�ڽ��շ�����������һ֡δ�����չ�
					{
						dbg_frame("Recv DATA %d %d, ID %d\n",f.seq, f.ack, *(short *)f.info);
						arrived[f.seq%NR_BUFS] = true; //��Ǹ�֡Ϊ�ѽ���
						memcpy(in_buffer[f.seq % NR_BUFS], f.info,PKT_LEN); //�����յ���֡��data���������뻺����
						while (arrived[frame_expected % NR_BUFS])//���յ����շ������½��һ֡ʱ������һ֡�Լ�֮���յ���֡���д���
						{
							put_packet(in_buffer[frame_expected %NR_BUFS], len - 7); //�����뻺�����������
							no_nak = true;
							arrived[frame_expected % NR_BUFS] = false;
							inc(frame_expected); //���շ������½�ģ��MAX_SEQ+1)��1
							inc(too_far);//���շ������Ͻ�ģ��MAX_SEQ+1)��1 
							start_ack_timer(ACK_TIMER); //���ack_timer��ʱ����ack֡
						}
					}
				}

				if ((f.kind == FRAME_NAK) && between(ack_expected, (f.ack + 1) % (MAX_SEQ + 1), next_frame_to_send))
				//����յ�����nak֡��ack����һ֡�ڷ��ͷ����������ack����һ֡
				{
					send_data_frame((f.ack + 1) % (MAX_SEQ + 1));
					dbg_frame("Recv NAK with ACK %d\n", f.ack);
				}

				while (between(ack_expected, f.ack, next_frame_to_send))
				//�ۼ�ȷ�ϣ�ֻҪack�ڷ��ͷ������ھͲ��ϵؽ�����ǰ��ֱ��δȷ�ϵ�һ֡
				{
					nbuffered--;
					stop_timer(ack_expected%NR_BUFS);
					inc(ack_expected);
				}
				break;

			case ACK_TIMEOUT://ack_timer��ʱʱ���Ͷ�����ack֡
				dbg_event("---- DATA %d timeout\n", oldest_frame);
				send_ack_frame();
				break;

			case DATA_TIMEOUT: //ĳ֡��data_timer��ʱ˵��δ�յ����շ���ack֡�������·��͸�֡
				dbg_event("---- DATA %d timeout\n", oldest_frame);
				if (!between(ack_expected, oldest_frame, next_frame_to_send))
					oldest_frame = oldest_frame + NR_BUFS;
				send_data_frame(oldest_frame);
				break;
		}

		if (nbuffered < NR_BUFS && phy_ready) //���������ready��Ŀǰ����δ������ʹ�����ready
			enable_network_layer();
		else
			disable_network_layer();
	}
}