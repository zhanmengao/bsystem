using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ClassLibrary1
{
    [JsonObject(MemberSerialization.OptOut)]
    public class RemoteAppInfo : INotifyPropertyChanged
    {
        [JsonProperty(PropertyName = "index")]
        public int Index { get; set; }

        [JsonProperty(PropertyName = "name")]
        public string Name { get; set; }

        [JsonProperty(PropertyName = "path")]
        public string Path { get; set; }

        [JsonProperty(PropertyName = "command_line_setting")]
        public string CommandLineSetting { get; set; }

        [JsonProperty(PropertyName = "required_command_line")]
        public string RequiredCommandLine { get; set; }

        [JsonProperty(PropertyName = "description")]
        public string Description { get; set; }

        [JsonIgnore]
        [JsonProperty(PropertyName = "image_base_64_str")]
        public object ImageStr { get; set; }


        public event PropertyChangedEventHandler PropertyChanged;

        public override bool Equals(object obj)
        {
            RemoteAppInfo info = obj as RemoteAppInfo;
            return (info!=null) && info.Path == info.Path;
        }
    }
}
