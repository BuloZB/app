///
//  macos_mm.m
//  aura
//
//  Created by Camilo Sasuke Tsumanuma on 3/8/16.
//
//

#import <Foundation/Foundation.h>



char * ns_get_default_browser_path()
{
   
   CFURLRef appURL = LSCopyDefaultApplicationURLForURL((__bridge CFURLRef)[NSURL URLWithString: @"http:"], kLSRolesAll, NULL);
   
   CFStringRef str = CFURLGetString(appURL);
   
   char * psz = strdup([(__bridge NSString *)str UTF8String]);
   
   CFRelease(appURL);
   
   //CFRelease(str);
   
   return psz;
   
}


bool ns_open_file(const char * psz)
{
   
   NSString * path = [NSString stringWithUTF8String:psz];
   
   if(path == NULL)
   {
      
      return false;
      
   }
   
   if(![[NSWorkspace sharedWorkspace] openFile: path])
   {
      
      return false;
      
   }
   
   return true;
   
}


void ns_launch_app_at_url(NSURL * url, const char ** argv)
{
   
   NSWorkspace * workspace = [NSWorkspace sharedWorkspace];
   
   NSMutableDictionary * dict = nil;
   
   if(argv != NULL)
   {
   
      NSMutableArray * array = [[NSMutableArray alloc] init];
   
      while(*argv != NULL)
      {
      
         [array addObject: [[NSString alloc] initWithUTF8String: *argv]];
         
         argv++;
         
      }

      dict = [[NSMutableDictionary alloc] init];

      [dict setObject:array forKey:NSWorkspaceLaunchConfigurationArguments];
      
   }
   
   //NSWorkspaceLaunchWithoutActivation
   
   [[workspace dd_invokeOnMainThread] launchApplicationAtURL:url options:NSWorkspaceLaunchDefault configuration:dict error:nil];

}


void ns_launch_app(const char * psz, const char ** argv)
{
   
   NSString * path = [[NSString alloc] initWithUTF8String:psz];
   
   NSURL * url = [NSURL fileURLWithPath:path isDirectory:NO];
   
   ns_launch_app_at_url(url, argv);
   
}


void ns_launch_bundle(const char * pszBundle, const char ** argv)
{

   NSString * bundle = [[NSString alloc] initWithUTF8String:pszBundle];
   
   NSURL * url = [[NSWorkspace sharedWorkspace] URLForApplicationWithBundleIdentifier:bundle];

   ns_launch_app_at_url(url, argv);
                        
}







bool GetImagePixelData(unsigned int * pcr, int cx, int cy, int iScan, CGImageRef inImage)
;




bool mm2_get_file_image(unsigned int * pcr, int cx, int cy, int iScan, const char * psz)
{
   
   NSString * str = [NSString stringWithUTF8String:psz];
   
   if(str == NULL)
   {
      
      return false;
      
   }
   
   NSImage *image = [[NSWorkspace sharedWorkspace] iconForFile:str];
   if(image == NULL)
   {
      
      return false;
      
   }
   
   NSRect r;
   
   r.origin.x = 0;
   r.origin.y = 0;
   r.size.width = cx;
   r.size.height = cy;
   
   CGImageRef i = [image CGImageForProposedRect: &r context:nil hints:nil];
   
   bool bOk = GetImagePixelData(pcr, cx, cy, iScan,  i);
   
   //CFRelease(i);
   
   return bOk;
   
   
   
}





bool mm1_get_file_image(unsigned int * pcr, int cx, int cy, int iScan, const char * psz)
{
   
   return mm2_get_file_image(pcr, cx, cy, iScan, psz);
}




void ns_app_terminate()
{
   
   [NSApp terminate:nil];
   
}






void ns_Sleep(unsigned int uiMillis)
{

   [NSThread sleepForTimeInterval: ((double) uiMillis / 1000.0) ];

}







//https://stackoverflow.com/questions/14699604/replacements-for-getmacosstatuserrorstring-getmacosstatuscommentstring

//11
//down vote
//accepted
//You can use [NSError errorWithDomain:NSOSStatusErrorDomain code:errorCode userInfo:nil] to get an NSError representing the OSStatus of errorCode. NSError provides the ability to get a localized error message from the error code (-localizedDescription), or a regular unlocalized one for debugging purposes (-description).

// GetMacOSStatusErrorString and GetMacOSStatusCommentString

char * mm_error_string(OSStatus status)
{
   
   NSError * error = [NSError errorWithDomain:NSOSStatusErrorDomain code: status userInfo: nil];
   
   NSString * strError = [error localizedDescription];
   
   return ns_string(strError);
   
}


char * mm_error_description(OSStatus status)
{
   
   NSError * error = [NSError errorWithDomain:NSOSStatusErrorDomain code: status userInfo: nil];
   
   NSString * strError = [error description];
   
   return ns_string(strError);
   
}




