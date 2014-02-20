using System;

namespace Xmlx
{
	public class HelloWorld : System.Web.UI.Page
	{

		public HelloWorld(){
			this.Load += new System.EventHandler(this.Page_Load);
		}

		private void Page_Load(object sender, System.EventArgs e){

			Response.Write("<html><body><p>Hello World!</p></body></html>");
		}
	}
}