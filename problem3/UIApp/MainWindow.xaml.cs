using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace UIApp
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        private Dictionary<string, IntPtr> loadedPlugins = new Dictionary<string, IntPtr>();
        private string imgFile = "";
        public MainWindow()
        {
            InitializeComponent();
        }

        private void button_sel_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "bmp file(*.bmp) | *.bmp";
            openFileDialog.FilterIndex = 1;
            openFileDialog.RestoreDirectory = true;
            string selected = Title;
            if (openFileDialog.ShowDialog() != true)
                return;
            Title =  selected = openFileDialog.FileName;
            imgFile = selected;
            BitmapImage bitmap = new BitmapImage();
            bitmap.BeginInit();
            bitmap.UriSource = new Uri(selected,UriKind.Absolute);
            bitmap.EndInit();
            image.Source = bitmap;
        }

        private void button_load_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "plugin dll(*.dll) | *.dll";
            openFileDialog.FilterIndex = 1;
            openFileDialog.RestoreDirectory = true;
            string selected = "";
            if (openFileDialog.ShowDialog() != true)
                return;
            selected = openFileDialog.FileName;
            IntPtr hModule = PluginDllUtils.LoadLibrary(selected);
            if (hModule == IntPtr.Zero)
                return;
            string pluginName = PluginDllUtils.GetPluginName(hModule);
            if(pluginName ==null|| pluginName.Substring(0, 5) != "PIMG_")
            {
                PluginDllUtils.FreeLibrary(hModule);
                return;
            }
            foreach(ListBoxItem item in listBox.Items)
            {
                string s = item.Content.ToString();
                if(s == pluginName)
                {
                    PluginDllUtils.FreeLibrary(hModule);
                    return;
                }
            }
            ListBoxItem newItem = new ListBoxItem();
            newItem.Content = pluginName;
            listBox.Items.Add(newItem);
            loadedPlugins[pluginName] = hModule;
        }
        delegate int FnProcess([MarshalAs(UnmanagedType.LPWStr)]string s);
        private void button_call_Click(object sender, RoutedEventArgs e)
        {
            if(imgFile!="")
            {

                ListBoxItem selected = (ListBoxItem)listBox.SelectedItem;
                if (selected == null)
                {
                    listBox.SelectedIndex = 0;
                    selected = (ListBoxItem)listBox.SelectedItem;
                }
                if (selected == null)
                    return;
                string s = selected.Content.ToString();
                if (loadedPlugins.ContainsKey(s))
                {
                    IntPtr hModule = loadedPlugins[s];
                    FnProcess fnProcess = (FnProcess)PluginDllUtils.GetProcDelegate(hModule, "Process", typeof(FnProcess));
                    if(fnProcess(imgFile)==1)//succeeded
                    {
                        imgFile = imgFile.Substring(0, imgFile.Length - 4) + "_processed.bmp";
                        BitmapImage bitmap = new BitmapImage();
                        bitmap.BeginInit();
                        bitmap.UriSource = new Uri(imgFile, UriKind.Absolute);
                        bitmap.EndInit();
                        image.Source = bitmap;
                        Title = imgFile;
                    }
                }
            }

        }
    }
}
