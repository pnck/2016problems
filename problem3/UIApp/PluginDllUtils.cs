using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace UIApp
{

    public class PluginDllUtils
    {
        [DllImport("kernel32.dll")]
        public static extern IntPtr LoadLibrary(string dllToLoad);
        [DllImport("kernel32.dll",CharSet =CharSet.Ansi)]
        public static extern IntPtr GetProcAddress(IntPtr hModule, string procedureName);
        [DllImport("kernel32.dll")]
        public static extern bool FreeLibrary(IntPtr hModule);


        public static Delegate GetProcDelegate(IntPtr hModule,string fn,Type t)
        {
            IntPtr addr = GetProcAddress(hModule, fn);
            if (addr == IntPtr.Zero)
                return null;
            else
                return Marshal.GetDelegateForFunctionPointer(addr, t);
        }

        delegate IntPtr FnGetPluginName();
        public static string GetPluginName(IntPtr hModule)
        {
            if (hModule == IntPtr.Zero)
                return null;
            FnGetPluginName fnGetPluginName = (FnGetPluginName)GetProcDelegate(hModule, "GetPluginName", typeof(FnGetPluginName));
            if (fnGetPluginName == null)
                return null;
            IntPtr pstr = fnGetPluginName();
            return Marshal.PtrToStringUni(pstr);
        }

    }
}
