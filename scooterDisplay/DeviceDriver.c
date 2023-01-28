/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software is delivered "as is" and shows the usage of other software
* components. It is provided as an example software which is intended to be
* modified and extended according to particular requirements.
*
* TARA Systems hereby disclaims all warranties and conditions with regard to the
* software, including all implied warranties and conditions of merchantability
* and non-infringement of any third party IPR or other rights which may result
* from the use or the inability to use the software.
*
********************************************************************************
*
* DESCRIPTION:
*   This file implements an interface between an Embedded Wizard generated UI
*   application and a certain device. Please consider this file only as template
*   that is intended to show the binding between an UI application and the
*   underlying system (e.g. middleware, BSP, hardware driver, protocol, ...).
*
*   This device driver is the counterpart to a device class implemented within
*   your Embedded Wizard project.
*
*   Feel free to adapt this file according your needs!
*
*   Within this sample, we demonstrate the access to the board LED and the
*   hardware button. Furthemore, the console interface is used to print a string.
*
*   This file assumes to be the counterpart of the device class 'DeviceClass'
*   within the unit 'Application'.
*   In order to ensure that this file can be compiled for all projects with or
*   without the device class, the generated define '_ApplicationDeviceClass_'
*   is used.
*
*******************************************************************************/

/*
   Include all necessary files to access the real device and/or to get access
   to the required operating system calls.
*/
#include "ewrte.h"
#include "ew_bsp_inout.h"
#include "ew_bsp_clock.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_adc.h"
/*
   Include the generated header file to access the device class, for example to
   access the class 'DeviceClass' from the unit 'Application' include the
   generated file 'Application.h'.
*/
//#include "Application.h"
#include "DeviceDriver.h"

ADC_HandleTypeDef hadc3;
#ifdef _ApplicationDeviceClass_

/*
   Create a static variable to keep the global instance (autoobject) of the
   device class. The type of the variable has to correspond to the device class
   and the unit name, for example the static variable of the class 'DeviceClass'
   from the unit 'Application' has the type 'ApplicationDeviceClass'.
*/
  static ApplicationDeviceClass DeviceObject = 0;

  /* variable to store the current hardware button state */
  static int IsHardButtonDown = 0;

  /* variable to count while Button is pressed */
  static int ButtonCounter = 0;

#endif


/*******************************************************************************
* FUNCTION:
*   HardButtonIsrCallback
*
* DESCRIPTION:
*   Callback function for the hardware button. This function is called every time
*   the hardware button is pressed or released. Please note, that this function is
*   called from the interrupt service routine.
*
* ARGUMENTS:
*   aButtonPresssed - 1, if hardware button is pressed and 0, if it is released.
*
*******************************************************************************/
void HardButtonIsrCallback( int aButtonPresssed )
{

#ifdef _ApplicationDeviceClass_

  /* store the current button state */
  IsHardButtonDown = aButtonPresssed;

  /*
     Important note: This function is called from an interrupt handler and not
     in the context of the main GUI task/thread/process. NEVER make a direct
     function call to a method of the driver class or any other generated code
     from an interrupt handler or another task/thread/process.
  */

#endif

}


/*******************************************************************************
* FUNCTION:
*   DeviceDriver_Initialize
*
* DESCRIPTION:
*   The function DeviceDriver_Initialize() initializes the module and prepares all
*   necessary things to access or communicate with the real device.
*   The function has to be called from your main module, after the initialization
*   of your GUI application.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void DeviceDriver_Initialize( void )
{
  /*
     You can implement here the necessary code to initialize your particular
     hardware, to open needed devices, to open communication channels, etc.
  */
	GPIO_InitTypeDef init;
  init.Mode = GPIO_MODE_OUTPUT_PP;
  init.Speed = GPIO_SPEED_HIGH;
  /* D0 => PC7 */
  init.Pin = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOC, &init);
  /* D1 => PC6 */
  init.Pin = GPIO_PIN_6;
  HAL_GPIO_Init(GPIOC, &init);
  /* D2 => PG6 */
  init.Pin = GPIO_PIN_6;
  HAL_GPIO_Init(GPIOG, &init);

  /* don't forget the pin clocks */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  
  // Configure ADC3
  
  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
    hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    return 0;
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
   sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  
   if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    return 0;
  }
  
   GPIO_InitTypeDef GPIO_InitStruct = {0};
  //if(hadc3->Instance==ADC3)
  //{
  /* USER CODE BEGIN ADC3_MspInit 0 */

  /* USER CODE END ADC3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_ADC3_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();

    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**ADC3 GPIO Configuration
    PF7     ------> ADC3_IN5
    PF6     ------> ADC3_IN4
    PF10     ------> ADC3_IN8
    PF9     ------> ADC3_IN7
    PF8     ------> ADC3_IN6
    PA0/WKUP     ------> ADC3_IN0
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
   
	
	/*  GPIO_InitStruct.Pin = ARDUINO_A0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ARDUINO_A0_GPIO_Port, &GPIO_InitStruct); */
  
   
	/* configure LED */
	EwBspInOutInitLed();
	HAL_ADC_Start(&hadc3);
	/* Configure interrupt for hardware button */
	EwBspInOutInitButton( HardButtonIsrCallback );

#ifdef _ApplicationDeviceClass_

  /*
     Get access to the counterpart of this device driver: get access to the
     device class that is created as autoobject within your Embedded Wizard
     project. For this purpose you can call the function EwGetAutoObject().
     This function contains two paramters: The pointer to the autoobject and
     the class of the autoobject.
     Assuming you have implemented the class 'DeviceClass' within the unit
     'Application' and you have an autoobject with the name 'Device', make
     the following call:
     EwGetAutoObject( &ApplicationDevice, ApplicationDeviceClass );
  */

  DeviceObject = EwGetAutoObject( &ApplicationDevice, ApplicationDeviceClass );

  /*
     Once you have the access to the autoobject, lock it as long as you need
     the access (typically, until your device driver is closed). By locking
     the autoobject, you can ensure that the object of the device class will
     be kept within the memory and not freed by the Garbage Collector.
     Once the device class is locked, you can easily store it within a static
     variable to access the driver class during the runtime.
  */

  EwLockObject( DeviceObject );

#endif

}


/*******************************************************************************
* FUNCTION:
*   DeviceDriver_Deinitialize
*
* DESCRIPTION:
*   The function DeviceDriver_Deinitialize() deinitializes the module and
*   finalizes the access or communication with the real device.
*   The function has to be called from your main module, before the GUI
*   application will be deinitialized.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void DeviceDriver_Deinitialize( void )
{
  /*
     You can implement here the necessary code to deinitialize your particular
     hardware, to close used devices, to close communication channels, etc.
  */

#ifdef _ApplicationDeviceClass_

  /*
     If you have access to the device class autoobject, don't forget to unlock
     it. Clear the static variable to avoid later usage.
  */

  if ( DeviceObject )
    EwUnlockObject( DeviceObject );

  DeviceObject = 0;

#endif

}


/*******************************************************************************
* FUNCTION:
*   DeviceDriver_ProcessData
*
* DESCRIPTION:
*   The function DeviceDriver_ProcessData() is called from the main UI loop, in
*   order to process data and events from your particular device.
*   This function is responisble to update properties within the device class
*   if the corresponding state or value of the real device has changed.
*   This function is also responsible to trigger system events if necessary.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   The function returns a non-zero value if a property has changed or if a
*   system event was triggered. If nothing happens, the function returns 0.
*
*******************************************************************************/
int DeviceDriver_ProcessData( void )
{
  int needUpdate = 0;

  /*
     Get the data you want to provide to the GUI application.
     In case your are working with an operating system and your device is
     controlled from a separate task/thread/process, take all information
     from your device driver out of the message queue.
     Please note, that this function is called within the context of the main
     GUI thread.
     If you control your system by direct register access or some BSP functions,
     get all necessary data you want to provide to the GUI application.
  */

#ifdef _ApplicationDeviceClass_

  /* here we just evaluate the current hardware button state */
  if ( IsHardButtonDown )
    ButtonCounter++;

  /* check for a valid access to the autoobject of the device class */
  if ( DeviceObject == 0 )
    return 0;

  /*
     For each device paramter, that is represented by a property within the
     Embedded Wizard device class and that you want to update, you have to call
     the appropriate UpdateProperty() method.

     The following examples assumes, that you have a device class with the
     name 'DeviceClass' within the unit 'Application'.
  */

  /* Update the property HardButtonCounter within the class Application::DeviceClass
     by calling the method 'UpdateHardButtonCounter' - the generated define is
     evaluated to ensures that the method is available within the generated code. */
  if ( ButtonCounter > 0 )
  {
    /* clear counter if button is released */
    if ( !IsHardButtonDown )
      ButtonCounter = 0;

    #ifdef _ApplicationDeviceClass__UpdateHardButtonCounter_

      ApplicationDeviceClass__UpdateHardButtonCounter( DeviceObject, (XInt32)ButtonCounter );
      needUpdate = 1;

    #endif
  }

  /*
     Trigger system events if necessary, e.g. if a certain situation happens,
     if an error occurs or just if a certain value has changed...
  */

  /* When the hardware button is pressed, call the method 'TriggerHardButtonEvent()' of the
     device class 'DeviceClass' within the unit 'Application' - the generated define is
     evaluated to ensure that the method is available within the generated code. */
  if ( ButtonCounter == 1 )
  {
    #ifdef _ApplicationDeviceClass__TriggerHardButtonEvent_

      ApplicationDeviceClass__TriggerHardButtonEvent( DeviceObject );
      needUpdate = 1;

    #endif
  }

#endif

  /*
     Return a value != 0 if there is at least one property changed or if a
     system event was triggered. The return value is used by the main loop, to
     decide whether the GUI application has changed or not.
  */

  return needUpdate;
}


/*******************************************************************************
* FUNCTION:
*   DeviceDriver_SetLedStatus
*
* DESCRIPTION:
*   This is a sample for a function called from the device class, when a
*   property has changed. As a result, the corresponding value of the real
*   device should be changed.
*   In this implementation simply the LED is switched on or off.
*
*******************************************************************************/
void DeviceDriver_SetLedStatus( XInt32 aLed, XBool aState )
{
  /*
     In case you are using an operating system to communicate with your
     device driver that is running within its own thread/task/process,
     send a message to the device driver and transmit the new value.
     Please note, that this function is called within the context of the main
     GUI thread.
  */

  /*
     Here we are accessing directly the device driver by calling a certain
     BSP / driver function.
  */
 switch ( aLed )
  {
    case 0:
      /* LED0 => PG7 */
      HAL_GPIO_WritePin( GPIOG, GPIO_PIN_6, aState ? GPIO_PIN_SET : GPIO_PIN_RESET);
      break;
    case 1:
      /* LED1 => PB4 */
      HAL_GPIO_WritePin( GPIOC, GPIO_PIN_6, aState ? GPIO_PIN_SET : GPIO_PIN_RESET);
      break;
    case 2:
      /* LED2 => PG6 */
      HAL_GPIO_WritePin( GPIOC, GPIO_PIN_7, aState ? GPIO_PIN_SET : GPIO_PIN_RESET);
      break;
  }
 
}

/*******************************************************************************
* FUNCTION:
*   DeviceDriver_readADC
*
* DESCRIPTION:
*   This is a sample for a function which read the adc value from
*   uC pins
*   
*
*******************************************************************************/

int DeviceDriver_readADC()
{
	 int value = HAL_ADC_GetValue(&hadc3);
	 if(value > 422)
		EwBspInOutLedOn();
	 return value;
}




/*******************************************************************************
* FUNCTION:
*   DeviceDriver_SetTime
*
* DESCRIPTION:
*   This is a sample for a function called from the device class, when the
*   system time (RTC time) should be changed.
*
*******************************************************************************/
void DeviceDriver_SetTime( XUInt32 aTime )
{
  /*
     In case you are using an operating system to communicate with your
     device driver that is running within its own thread/task/process,
     send a message to the device driver and transmit the new value.
     Please note, that this function is called within the context of the main
     GUI thread.
  */

  /*
     Here we are accessing directly the device driver by calling a certain
     BSP / driver function.
  */

  EwBspClockSetTime( aTime );
}


/* msy */
