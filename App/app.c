/**
 * @file app.c
 * @date: 4/26/2026
 * @author: Cory W. Hodge
 * @brief: Application entry point for the RV_Thermostat project.
 */

 // for tasks
 #include "tasks.h"
 
 /**
  * application_entry
  * @brief: Application entry point for the RV_Thermostat project.
  * BEGINNING OF APPLICATION
  */
 void application_entry(void)
 {
    // Configuration and initialization code for the application can be added here.

    // tasks are organized into the tasks.c file
    startTasks();
 };