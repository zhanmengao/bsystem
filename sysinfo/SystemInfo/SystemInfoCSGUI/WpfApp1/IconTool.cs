using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Media.Imaging;

namespace WpfApp1
{
    class IconTool
    {
        private const int ICON_SIZE = 128;
        private static BitmapImage BitmapToBitmapImage(Bitmap bitmap)
        {
            BitmapImage bitmapImage = new BitmapImage();
            using (MemoryStream ms = new MemoryStream())
            {
                bitmap.Save(ms, bitmap.RawFormat);
                bitmapImage.BeginInit();
                bitmapImage.StreamSource = ms;
                bitmapImage.CacheOption = BitmapCacheOption.OnLoad;
                bitmapImage.EndInit();
                bitmapImage.Freeze();
            }
            return bitmapImage;
        }

        public static string GetImageBase64String(string exeFilePath)
        {
            Bitmap bitmap = GetBitmap(exeFilePath);

            return bitmap != null ? BitmapToBase64String(bitmap) : "iVBORw0KGgoAAAANSUhEUgAAABkAAAAZCAYAAADE6YVjAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsQAAA7EAZUrDhsAAAacSURBVEhLfVVpbFRVFP7eMjOdpUwXpOxCJ4garVLjQqIxaqIG9Ycmrr+EqAguP9RoTDRoosZEE5cYd0RBqxhxVzY1VSEFgdBYhAZboJRus3Vm3rx5+3t+9xUETfU0nblz333nO+d83zlX8nw/wP+YLEnHVxPG85D+tXeqiSdZ3eI3/yQftsX1ZCBBEECR5XDdd2gAhXweqVQ9zjozA1lW4PH5ZDA+91VZQkE3eEYhSADbticHEdHv2NODF95aj4phw3NMGJaH8ngeSy6/CC8+/fB/Aom9sunA9iYydlwPE+GeYgLg1fe/xBsffodpzU1Ip5KIMLo6xUVTUyM2/roLX32zCQrPiYwnMxHACZMRnAQR2yHAmi+whY6ap7UgVyxA1yvkwYUTMComXV+fwpMvrcHbH6wPSzo5zEnz+M7fICKyPwdG8e1PXdC0Ckq5UTTHVaSjEppiElqnKJDNCvyahnhUxdOvrmVkfvjeP4w/BekTywAugwtBTqjlmTe/wPy4CZt1XLZkEZrrAsR8EzIz6T2Wh+H6CHjW8VzElQAH+wfC9041AeD4XrgSrAkAWaQrIFZ//jOOHD3M2kton5HEhedk8NAd12DRwrmwLJMR+5jTEA8VJM7YfDE/NsY3T5qgoma7XAn5Um2Uewgk0s2WdGzq3AnX0mE4HpZcvBCx6TMwXiwhNzyEOxYvwHXtGbTNbqQjap+ZqvCwdduuCe/HTaFAqqIvwl8SfM/h53FOtu/pRbVagkQH3SM6WmdNFYwhmUrhhsvacXbbuVh++3UYHM2hatqY15SAxJJ8u7UzdCdMKK1Qs+lb9IcAsGH7avgsBMnmCghcJ+zmVExBJBIBHAsKv1P19ZifacUvP3ZidkMCd184B9lCEQpL1nuwH67rhqqsWjZcj1yIOhFQ8gnIPFRPnwCRmaaoNXmFxZrqpgWbjsrsdIn7Zq2GXC6PhkQMh6jAgaMDUN0aptQnsebjz2GQC9MjD5wGojyKV0VgW4jyWw6ON+P82dOoRqEIVsmu4diRI9jXsw++4yCRTKBryw9oW5BBZuoUfP9HP6DGmLmNRFTBe+s+RZzBS4EDmVF7lLlpuWSMPURVhn1ikqgUG0zUUfYsNp2M7j8HoagqysUivl63HolUHMMs6RMdmxlnEE4AoSRV8rC7uwe//LoNSbOApJFD2i6gwSujCRpaVAstMZbTIJGfbexCQzqNZCKOSKwO+w8cwd6du1HQyvAa6vH+pi4sfbkDxaqBiKJQmw5mzpqLkeFRPH//bbgkcxoSsoMYGzcSSyEq/lNpSPEkpGidKHkQjI/nsPKpd5EdG+YwNJDgQMyMHsYQsxusmihREBqDERO2PhlHjZydu6AVH72yCmnOM4OcBZR1NEWHJMAP6iBHVEiqEqounMJCHR45Wfnos9hPxRQrVZYESGlZNMLFGHtnsGJAVWREVRlJqq57awfB7NBJbmQElXwObYsvIRduqDyP8pV4HlRcSLxQlkJlvPXi41iyuA333nQFssPH4PMuELrPxCKo8ozo4NFsAVs7XmPnKVAJKPYMvYo9O3ehQgUGtg9XjAM2YmCb8Mh5CCL07JtFoHAUhVIJDzx4J1Y//zjOaW/HBaz/XKa9YmYjhgeGsHnty2hZ2IpAJqHcd1kmn24UNYKN33yP7NAItPw49LKOarkGraJDWbVq1VMCJGDzITUVi84/D+s3bMYtt95I9dj47YdORDnS5bKGFY/cg+uX3gXfMsBwKVH2EC81vaazUWr4/UAfhg4fIh8K553NnnNQ0bRTb0Yh9okRcLC7Cxu+3IJlN1+Lzt09ePS513H1BWfjnY4P4ZWHSS5rbmi8Wn3o5G8sO4bCsUEc2LcfDeddilp+FJJehMoqJEV7nAARfSIGmxgt4jJa995qzGyegtY507FlRw+Wr7wTO9euRd/evbhq+XJMO60ReYrBocyLnA4lToe+3l6kz788HJRyPM0SxuBUixMgAkDMoOF8FadPbxCY6N++CbMXnBEOSpfKcylbnw3qcQoURrNQo5xrvJp1yrc8XoZRKWEkm4PVNA+JeDy80ALqWSFY2CfC6UMvfY3MjBTuu+3KEKRv188YZ3TxZJKXlgePBCt1MVLhkCsJI4NHYVd1ROgs1tyMKPsiVyojr05FjJzEEkkKUKH6KOHHXtscOm2qs/DBR59w5cAY60fLrJmhTCtUm8ZZZPEatdkXRs2ARueGaQLxOjicEGOHBzBIPlTK1tEKHOAOzKoWSjuAhL8A+WJ55+P9pSMAAAAASUVORK5CYII=";

        }
        /*
        private void SaveImageFile(Bitmap bitmap)
        {
            bitmap.Save("pantyhosera66iss" + ".png", ImageFormat.Png);
        }
        */
        private static Bitmap GetBitmap(string exeFilePath)
        {
            IntPtr[] hIcons = new IntPtr[1];
            int[] ids = new int[1];
            var successCount = PrivateExtractIcons(exeFilePath, 0, ICON_SIZE, ICON_SIZE, hIcons, ids, 1, 0);
            if (successCount > 0)
            {
                var ico = Icon.FromHandle(hIcons[0]);
                Bitmap myIcon = ico.ToBitmap();
                //内存回收
                DestroyIcon(hIcons[0]);
                return myIcon;
            }
            else
            {
                return null;
            }
        }

        private static string BitmapToBase64String(Bitmap bitmap)
        {
            try
            {
                MemoryStream ms = new MemoryStream();
                bitmap.Save(ms, ImageFormat.Png);
                byte[] arr = new byte[ms.Length];
                ms.Position = 0;
                ms.Read(arr, 0, (int)ms.Length);
                ms.Close();
                return Convert.ToBase64String(arr);
            }
            catch (Exception exception)
            {
                return null;
            }
        }

        //details: https://msdn.microsoft.com/en-us/library/windows/desktop/ms648075(v=vs.85).aspx
        //Creates an array of handles to icons that are extracted from a specified file.
        //This function extracts from executable (.exe), DLL (.dll), icon (.ico), cursor (.cur), animated cursor (.ani), and bitmap (.bmp) files. 
        //Extractions from Windows 3.x 16-bit executables (.exe or .dll) are also supported.
        [DllImport("User32.dll")]
        private static extern int PrivateExtractIcons(
            string lpszFile, //file name
            int nIconIndex,  //The zero-based index of the first icon to extract.
            int cxIcon,      //The horizontal icon size wanted.
            int cyIcon,      //The vertical icon size wanted.
            IntPtr[] phicon, //(out) A pointer to the returned array of icon handles.
            int[] piconid,   //(out) A pointer to a returned resource identifier.
            int nIcons,      //The number of icons to extract from the file. Only valid when *.exe and *.dll
            int flags        //Specifies flags that control this function.
        );

        //details:https://msdn.microsoft.com/en-us/library/windows/desktop/ms648063(v=vs.85).aspx
        //Destroys an icon and frees any memory the icon occupied.
        [DllImport("User32.dll")]
        private static extern bool DestroyIcon(
            IntPtr hIcon //A handle to the icon to be destroyed. The icon must not be in use.
        );


    }

}

