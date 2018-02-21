/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "gfx.h"
#include "stdlib.h"
#include "math.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId anotherTaskHandle;

/* USER CODE BEGIN Variables */
#define PI 3.14159265358979323846

#define SAMPLE_COUNT 200

typedef enum Waveform { // the different possible output
  SQUARE, // waveforms
  SINE,
  TRIANGLE,
  SAW,
  OFF
}
Waveform;

GHandle   ghLabel1;
GHandle   ghBtnSine;
GHandle   ghBtnTriangle;
GHandle   ghBtnSquare;
GHandle   ghBtnSaw;
GListener gl;
Waveform output_waveform = SAW;

int TRIANGLE_TABLE[200] = {
		0x0,0x1,0x1,0x1,0x2,0x2,0x2,0x3,
		0x3,0x3,0x4,0x4,0x4,0x4,0x5,0x5,
		0x5,0x6,0x6,0x6,0x7,0x7,0x7,0x8,
		0x8,0x8,0x9,0x9,0x9,0xa,0xa,0xa,
		0xb,0xb,0xb,0xc,0xc,0xc,0xc,0xd,
		0xd,0xd,0xe,0xe,0xe,0xf,0xf,0xf,
		0x10,0x10,0x10,0x11,0x11,0x11,0x12,0x12,
		0x12,0x13,0x13,0x13,0x14,0x14,0x14,0x14,
		0x15,0x15,0x15,0x16,0x16,0x16,0x17,0x17,
		0x17,0x18,0x18,0x18,0x19,0x19,0x19,0x1a,
		0x1a,0x1a,0x1b,0x1b,0x1b,0x1c,0x1c,0x1c,
		0x1c,0x1d,0x1d,0x1d,0x1e,0x1e,0x1e,0x1f,
		0x1f,0x1f,0x20,0x20,0x20,0x1f,0x1f,0x1f,
		0x1e,0x1e,0x1e,0x1d,0x1d,0x1d,0x1c,0x1c,
		0x1c,0x1c,0x1b,0x1b,0x1b,0x1a,0x1a,0x1a,
		0x19,0x19,0x19,0x18,0x18,0x18,0x17,0x17,
		0x17,0x16,0x16,0x16,0x15,0x15,0x15,0x14,
		0x14,0x14,0x14,0x13,0x13,0x13,0x12,0x12,
		0x12,0x11,0x11,0x11,0x10,0x10,0x10,0xf,
		0xf,0xf,0xe,0xe,0xe,0xd,0xd,0xd,
		0xc,0xc,0xc,0xc,0xb,0xb,0xb,0xa,
		0xa,0xa,0x9,0x9,0x9,0x8,0x8,0x8,
		0x7,0x7,0x7,0x6,0x6,0x6,0x5,0x5,
		0x5,0x4,0x4,0x4,0x4,0x3,0x3,0x3,
		0x2,0x2,0x2,0x1,0x1,0x1,0x0,0x0};
int SINE_TABLE[200] = {
		0x10,0x11,0x11,0x12,0x12,0x13,0x13,0x13,
		0x14,0x14,0x15,0x15,0x16,0x16,0x17,0x17,
		0x18,0x18,0x19,0x19,0x19,0x1a,0x1a,0x1b,
		0x1b,0x1b,0x1c,0x1c,0x1c,0x1d,0x1d,0x1d,
		0x1e,0x1e,0x1e,0x1e,0x1e,0x1f,0x1f,0x1f,
		0x1f,0x1f,0x1f,0x20,0x20,0x20,0x20,0x20,
		0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
		0x20,0x20,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,
		0x1e,0x1e,0x1e,0x1e,0x1e,0x1d,0x1d,0x1d,
		0x1c,0x1c,0x1c,0x1b,0x1b,0x1b,0x1a,0x1a,
		0x19,0x19,0x19,0x18,0x18,0x17,0x17,0x16,
		0x16,0x15,0x15,0x14,0x14,0x13,0x13,0x13,
		0x12,0x12,0x11,0x11,0x10,0xf,0xf,0xe,
		0xe,0xd,0xd,0xd,0xc,0xc,0xb,0xb,
		0xa,0xa,0x9,0x9,0x8,0x8,0x7,0x7,
		0x7,0x6,0x6,0x5,0x5,0x5,0x4,0x4,
		0x4,0x3,0x3,0x3,0x2,0x2,0x2,0x2,
		0x2,0x1,0x1,0x1,0x1,0x1,0x1,0x0,
		0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
		0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x1,
		0x1,0x1,0x1,0x1,0x2,0x2,0x2,0x2,
		0x2,0x3,0x3,0x3,0x4,0x4,0x4,0x5,
		0x5,0x5,0x6,0x6,0x7,0x7,0x7,0x8,
		0x8,0x9,0x9,0xa,0xa,0xb,0xb,0xc,
		0xc,0xd,0xd,0xd,0xe,0xe,0xf,0xf
};

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void StartAnotherTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */
void uGFXMain();
void createLabel();
void createButtonSine();
void createButtonTriangle();
void createButtonSquare();
void createButtonSaw();
int getNextValue(int time);
/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */

  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
	GEvent* pe;
	gfxInit();
	uGFXMain();
	geventListenerInit(&gl);
	gwinAttachListener(&gl);

	  /* definition and creation of anotherTask */
	  osThreadDef(anotherTask, StartAnotherTask, osPriorityNormal, 0, 128);
	  anotherTaskHandle = osThreadCreate(osThread(anotherTask), NULL);

	while(1) {
			// Get an Event
			pe = geventEventWait(&gl, TIME_INFINITE);

			switch(pe->type) {
				case GEVENT_GWIN_BUTTON:
					if (((GEventGWinButton*)pe)->gwin == ghBtnSine) {
						output_waveform = SINE;
						//generator.
					} else if (((GEventGWinButton*)pe)->gwin == ghBtnTriangle) {
						output_waveform = TRIANGLE;
					} else if (((GEventGWinButton*)pe)->gwin == ghBtnSquare) {
						output_waveform = SQUARE;
					} else if (((GEventGWinButton*)pe)->gwin == ghBtnSaw) {
						output_waveform = SAW;
					}
					break;
				default:
					HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
					gfxSleepMilliseconds(1000);
					HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_RESET);
					break;
			}
		}
  /* USER CODE END StartDefaultTask */
}

/* StartAnotherTask function */
void StartAnotherTask(void const * argument)
{
  /* USER CODE BEGIN StartAnotherTask */
	int counter = 0;
	int counter2 = 100;
	int startX = 5;
	int startY = 100;
	int oldX = startX;
	int oldY = startY;
  /* Infinite loop */
	for(;;) {

		int endY = startY;
		int endY2 = startY;

		endY -= getNextValue(counter);
		endY2 -= getNextValue(counter2);

		//gdispDrawPixel(startX + counter, endY, Blue);
		gdispDrawLine(startX + counter, endY, oldX, oldY, Blue);
		//gdispDrawPixel(startX + counter2, endY2, White);
		gdispDrawLine(startX + counter2, startY - 10, startX + counter2, startY + 10, White);

		oldX = startX + counter;
		oldY = endY;

		counter++;
		counter2++;
		if ((counter % SAMPLE_COUNT) == 0) {
			counter = 0;
			oldX = startX;
			oldY = startY;
		}
		if ((counter2 % SAMPLE_COUNT) == 0) {
					counter2 = 0;
				}
		gfxSleepMilliseconds(50);
	}
  /* USER CODE END StartAnotherTask */
}

/* USER CODE BEGIN Application */

int getNextValue(int t) {
	double value = 10;
	double rad = (PI*t)/180;
	switch(output_waveform) {
		case SINE:
			value = sin((float)(2*PI*rad));
			break;
		case TRIANGLE:
			//value = 1-4*(double)abs(round(t-0.25)-(t-0.25));
			value = (((t + 20) % 50) < 20)       * (((t + 20) % 50) * 1 / 20.0) +
					((t % 50) < 20) * 1 * (1 - ((t % 50) / 20.0));
			break;
		case SQUARE:
			//value = signbit(sin(2*PI*rad));
			value = ((t % 30) < 20) * 1;
			break;
		case SAW:
			//value = 2*(t-(float)floor(t+0.5));
			value = ((t % 30) < 20) * ((t % 30) * 1 / 20.0);
			break;
		default:
			return 0;
	}


	value *= 10;
	return (int) round(value);
}

void createLabel() {
	GWidgetInit		wi;

	// Apply some default values for GWIN
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	wi.g.show = TRUE;

	// Apply the label parameters
	wi.g.x = 10;
	wi.g.y = 200;
	wi.g.width = 100;
	wi.g.height = 20;
	wi.text = "Value: ";

	// Create the actual label
	ghLabel1 = gwinLabelCreate(NULL, &wi);
}

void createButtonSine() {
	GWidgetInit	wi;

	// Apply some default values for GWIN
	//gwinWidgetClearInit(&wi);
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	wi.g.show = TRUE;

	// Apply the button parameters
	wi.g.width = 100;
	wi.g.height = 30;
	wi.g.x = 10;
	wi.g.y = 5;
	wi.text = "Sine";

	// Create the actual button
	ghBtnSine = gwinButtonCreate(NULL, &wi);
}
void createButtonTriangle() {
	GWidgetInit	wi;

		// Apply some default values for GWIN
		//gwinWidgetClearInit(&wi);
		wi.customDraw = 0;
		wi.customParam = 0;
		wi.customStyle = 0;
		wi.g.show = TRUE;

		// Apply the button parameters
		wi.g.width = 100;
		wi.g.height = 30;
		wi.g.x = 120;
		wi.g.y = 5;
		wi.text = "Triangle";

		// Create the actual button
		ghBtnTriangle = gwinButtonCreate(NULL, &wi);
}
void createButtonSquare() {
	GWidgetInit	wi;

		// Apply some default values for GWIN
		//gwinWidgetClearInit(&wi);
		wi.customDraw = 0;
		wi.customParam = 0;
		wi.customStyle = 0;
		wi.g.show = TRUE;

		// Apply the button parameters
		wi.g.width = 100;
		wi.g.height = 30;
		wi.g.x = 10;
		wi.g.y = 35;
		wi.text = "Square";

		// Create the actual button
		ghBtnSquare = gwinButtonCreate(NULL, &wi);
}
void createButtonSaw() {
	GWidgetInit	wi;

		// Apply some default values for GWIN
		//gwinWidgetClearInit(&wi);
		wi.customDraw = 0;
		wi.customParam = 0;
		wi.customStyle = 0;
		wi.g.show = TRUE;

		// Apply the button parameters
		wi.g.width = 100;
		wi.g.height = 30;
		wi.g.x = 120;
		wi.g.y = 35;
		wi.text = "Saw";

		// Create the actual button
		ghBtnSaw = gwinButtonCreate(NULL, &wi);
}

void uGFXMain() {
	gwinSetDefaultFont(gdispOpenFont("UI2"));
	gwinSetDefaultStyle(&WhiteWidgetStyle, FALSE);
	gdispClear(White);

	// create the widget
	createButtonSine();
	createButtonTriangle();
	createButtonSquare();
	createButtonSaw();
	//createLabel();
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
