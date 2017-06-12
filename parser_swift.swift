import Foundation 
 
@discardableResult 
func shell(_ args: String...) -> Int32 { 
  let task = Process() 
  task.launchPath = "/usr/bin/env" 
  task.arguments = args 
  task.launch() 
  task.waitUntilExit()
  if task.terminationStatus == 0 { 
 	 print(" was found")
 	} 
  else{ 
 	print("Not Matched") 
   } 
  return task.terminationStatus  
 }  
 
 

shell("./parser","a+bc", "aabc")
