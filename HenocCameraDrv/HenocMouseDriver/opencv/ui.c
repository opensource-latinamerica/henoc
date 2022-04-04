#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ui.h"
#include "camera-photo.h"

static void capture_source_auto_detection(GtkIconView *iconview){
	GtkListStore *store;
	GtkTreeIter iter;
	store = gtk_list_store_new (3, G_TYPE_STRING, GDK_TYPE_PIXBUF,G_TYPE_STRING);
	GdkPixbuf *pixbuf;
	char buf[1024];
	
	int i;
	
	/// Camera Photo Image from Tango Project
	pixbuf = gdk_pixbuf_new_from_inline(-1,camera_photo,0,0);

	V4L2Capture *capture ;
	
	for (i=0;i<64;i++){
		snprintf(buf,sizeof(buf),"/dev/video%d",i);
		capture = v4l2CaptureOpen(buf);	
		if (capture){
		
			gtk_list_store_append(store , &iter);
			snprintf (buf,sizeof(buf),
			 	"%s \n[%s]", 
			 	v4l2CaputreGetName(capture),
			 	v4l2CaputreGetLocation(capture));
			 		
			gtk_list_store_set (store, &iter, 
				0, buf,
				1, pixbuf,
				2, v4l2CaputreGetLocation(capture),				
			 	-1);
			
			v4l2CaptureRelease(&capture);
		}
	}
	
  gtk_icon_view_set_model (GTK_ICON_VIEW (iconview), GTK_TREE_MODEL (store));
  gtk_icon_view_set_text_column   (GTK_ICON_VIEW (iconview), 0);
  gtk_icon_view_set_pixbuf_column   (GTK_ICON_VIEW (iconview), 1);
  
  g_object_unref (G_OBJECT (store));
  g_object_unref(G_OBJECT(pixbuf));
  
}

/** 
 * @param isV4L2Device - Return whether is the input source a V4L2 device
 * @Return A newly allocated string contains the location of the capture source(should be a file other a video device). It must be released by user.    
 */

char *capture_source_chooser_run(int *isV4L2Device){
	GtkWidget *CaptureSourceChooserDialog;
	GtkWidget *Notebook;
	GtkWidget *scrolledwindow3;
	GtkWidget *VideoDevicesIconview;
	GtkWidget *label;
	GtkWidget *FileChooser;
	GtkWidget *dialog_action_area1;
	GtkWidget *cancelbutton1;
	GtkWidget *okbutton1;
	  
	CaptureSourceChooserDialog = gtk_dialog_new ();
	gtk_window_set_title (GTK_WINDOW (CaptureSourceChooserDialog), "ConnectTo");
	gtk_window_set_type_hint (GTK_WINDOW (CaptureSourceChooserDialog), GDK_WINDOW_TYPE_HINT_DIALOG);
		
	GtkWidget *dialog_vbox1;
	dialog_vbox1 = GTK_DIALOG (CaptureSourceChooserDialog)->vbox;
	gtk_widget_show (dialog_vbox1);
		
	Notebook = gtk_notebook_new ();
	gtk_widget_show (Notebook);
	gtk_box_pack_start (GTK_BOX (dialog_vbox1), Notebook, TRUE, TRUE, 0);
	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (Notebook), GTK_POS_LEFT);
		  		
	scrolledwindow3 = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show (scrolledwindow3);
	gtk_container_add (GTK_CONTAINER (Notebook), scrolledwindow3);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow3), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolledwindow3), GTK_SHADOW_IN);
	
	VideoDevicesIconview = gtk_icon_view_new ();
	
	gtk_icon_view_set_selection_mode(GTK_ICON_VIEW(VideoDevicesIconview),
		GTK_SELECTION_SINGLE);
	
	gtk_icon_view_set_item_width(GTK_ICON_VIEW(VideoDevicesIconview),-1);
		
	gtk_widget_show (VideoDevicesIconview);
	gtk_container_add (GTK_CONTAINER (scrolledwindow3), VideoDevicesIconview);
	
	label = gtk_label_new ("Video Devices");
	gtk_widget_show (label);
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (Notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (Notebook), 0), label);
	
	FileChooser = gtk_file_chooser_widget_new (GTK_FILE_CHOOSER_ACTION_OPEN);
	gtk_widget_show (FileChooser);
	gtk_container_add (GTK_CONTAINER (Notebook), FileChooser);
	gtk_widget_set_size_request (FileChooser, 500, 300);
	
	label = gtk_label_new ("Multimedia Files");
	gtk_widget_show (label);
	gtk_notebook_set_tab_label (GTK_NOTEBOOK (Notebook), gtk_notebook_get_nth_page (GTK_NOTEBOOK (Notebook), 1), label);
	
	dialog_action_area1 = GTK_DIALOG (CaptureSourceChooserDialog)->action_area;
	gtk_widget_show (dialog_action_area1);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area1), GTK_BUTTONBOX_END);
	
	cancelbutton1 = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (cancelbutton1);
	gtk_dialog_add_action_widget (GTK_DIALOG (CaptureSourceChooserDialog), cancelbutton1, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS (cancelbutton1, GTK_CAN_DEFAULT);
	
	okbutton1 = gtk_button_new_from_stock ("gtk-ok");
	gtk_widget_show (okbutton1);
	gtk_dialog_add_action_widget (GTK_DIALOG (CaptureSourceChooserDialog), okbutton1, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);
		
	capture_source_auto_detection(GTK_ICON_VIEW(VideoDevicesIconview));
	
	char *filename = 0;
	*isV4L2Device = 0;
	
	if (gtk_dialog_run (GTK_DIALOG (CaptureSourceChooserDialog)) == GTK_RESPONSE_OK)	{
		int page = gtk_notebook_get_current_page(GTK_NOTEBOOK(Notebook));
		
		if (page==1) { // Multimedia File
			
			filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(FileChooser));
			
			
		} else if (page==0){
			GList * list = gtk_icon_view_get_selected_items(GTK_ICON_VIEW(VideoDevicesIconview));
			if (list){			
				GtkTreePath *path =(GtkTreePath*) list->data;
				gchar *location;
				GtkTreeIter iter;
				GtkTreeModel *model = gtk_icon_view_get_model(GTK_ICON_VIEW(VideoDevicesIconview));
				
				gtk_tree_model_get_iter(model,&iter,path);
				
				gtk_tree_model_get(model,&iter,2,&location,-1);
				
				filename = location;
				*isV4L2Device = 1;				
				
				g_list_foreach (list, (GFunc) gtk_tree_path_free, NULL);
				g_list_free (list);
			}
		}
	}
		
	gtk_widget_destroy(CaptureSourceChooserDialog);
	
	return filename;
}

static void change_image_format(GtkComboBox *combobox,void **data){
		V4L2Capture *dev = (V4L2Capture*) data[0];
		GtkWidget* ResolutionComboBox = (GtkWidget*) data[1];
	
		int* formats = v4l2CaptureGetImageFormatsList(dev);	
		int index = gtk_combo_box_get_active(GTK_COMBO_BOX(combobox));
		int i;
		int target;
		char buf[256];
		int res;
		
		target =formats[index];
		
		//printf("Choosed %s\n",fourcc_name(formats[index]));

		//Clear all items in the combo box
		GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(ResolutionComboBox));
		gtk_list_store_clear(GTK_LIST_STORE(model));		
	
		res = v4l2CaptureSetImageFormat(dev,target,0);
		
		if(!res){
			/* Resolutions List */
				
			VidSize *resolutions;
			v4l2CaptureQueryResolutionsList(dev,&resolutions);
			
			if (resolutions){
				i = 0;
				while (resolutions[i].width!=0){
					
					snprintf(buf,sizeof(buf),"%dx%d",
						resolutions[i].width,resolutions[i].height);
					gtk_combo_box_append_text(GTK_COMBO_BOX(ResolutionComboBox),buf);					
					i++;	
				}
				gtk_combo_box_set_active(GTK_COMBO_BOX(ResolutionComboBox),0);
			}
			
			g_free(resolutions);
		}
}

static V4L2Capture* real_video_tuning_dialog_run(V4L2Capture *dev,
	int *format,int *width,int *height,int *channel,int *norm,int *fps){
  GtkWidget *VideoTuningDialog;
  GtkWidget *dialog_vbox2;
  GtkWidget *table8;
  GtkWidget *label50;
  GtkWidget *label51;
  GtkWidget *DeviceNameEntry;
  GtkWidget *LocationEntry;
  GtkWidget *label55;
  GtkWidget *TVNormComboBox;
  GtkWidget *label54;
  GtkWidget *label58;
  GtkWidget *SourceComboBox;
  GtkWidget *FormatComboBox;
  GtkWidget *label53;
  GtkObject *FPSSpinButton_adj;
  GtkWidget *FPSSpinButton;
  GtkWidget *label52;
  GtkWidget *ResolutionComboBox;
  GtkWidget *label60;
  GtkWidget *DriverEntry;
  GtkWidget *dialog_action_area2;
  GtkWidget *CancelButton;
  GtkWidget *OKButton;
  
  VideoTuningDialog = gtk_dialog_new ();
  gtk_window_set_title (GTK_WINDOW (VideoTuningDialog), "Video Tuning");
  gtk_window_set_type_hint (GTK_WINDOW (VideoTuningDialog), GDK_WINDOW_TYPE_HINT_DIALOG);

  dialog_vbox2 = GTK_DIALOG (VideoTuningDialog)->vbox;
  gtk_widget_show (dialog_vbox2);

  table8 = gtk_table_new (6, 4, FALSE);
  gtk_widget_show (table8);
  gtk_box_pack_start (GTK_BOX (dialog_vbox2), table8, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (table8), 5);
  gtk_table_set_row_spacings (GTK_TABLE (table8), 3);
  gtk_table_set_col_spacings (GTK_TABLE (table8), 3);

  label50 = gtk_label_new ("Device Name");
  gtk_widget_show (label50);
  gtk_table_attach (GTK_TABLE (table8), label50, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label50), GTK_JUSTIFY_RIGHT);
  gtk_label_set_line_wrap (GTK_LABEL (label50), TRUE);
  gtk_misc_set_alignment (GTK_MISC (label50), 0, 0.5);

  label51 = gtk_label_new ("Location");
  gtk_widget_show (label51);
  gtk_table_attach (GTK_TABLE (table8), label51, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label51), GTK_JUSTIFY_RIGHT);
  gtk_misc_set_alignment (GTK_MISC (label51), 0, 0.5);

  DeviceNameEntry = gtk_entry_new ();
  gtk_widget_show (DeviceNameEntry);
  gtk_table_attach (GTK_TABLE (table8), DeviceNameEntry, 1, 4, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_editable_set_editable (GTK_EDITABLE (DeviceNameEntry), FALSE);

  LocationEntry = gtk_entry_new ();
  gtk_widget_show (LocationEntry);
  gtk_table_attach (GTK_TABLE (table8), LocationEntry, 1, 4, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_editable_set_editable (GTK_EDITABLE (LocationEntry), FALSE);

  label55 = gtk_label_new ("TV Norm");
  gtk_widget_show (label55);
  gtk_table_attach (GTK_TABLE (table8), label55, 0, 1, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label55), 0, 0.5);

  TVNormComboBox = gtk_combo_box_new_text ();
  gtk_widget_show (TVNormComboBox);
  gtk_table_attach (GTK_TABLE (table8), TVNormComboBox, 1, 2, 5, 6,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  //gtk_combo_box_append_text (GTK_COMBO_BOX (TVNormComboBox), "");

  label54 = gtk_label_new ("Source");
  gtk_widget_show (label54);
  gtk_table_attach (GTK_TABLE (table8), label54, 0, 1, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label54), 0, 0.5);

  label58 = gtk_label_new ("Format");
  gtk_widget_show (label58);
  gtk_table_attach (GTK_TABLE (table8), label58, 0, 1, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label58), 0, 0.5);

  SourceComboBox = gtk_combo_box_new_text ();
  gtk_widget_show (SourceComboBox);
  gtk_table_attach (GTK_TABLE (table8), SourceComboBox, 1, 2, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  //gtk_combo_box_append_text (GTK_COMBO_BOX (SourceComboBox), "");

  FormatComboBox = gtk_combo_box_new_text ();
  gtk_widget_show (FormatComboBox);
  gtk_table_attach (GTK_TABLE (table8), FormatComboBox, 1, 2, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  //gtk_combo_box_append_text (GTK_COMBO_BOX (FormatComboBox), "");

  label53 = gtk_label_new ("FPS");
  gtk_widget_show (label53);
  gtk_table_attach (GTK_TABLE (table8), label53, 2, 3, 4, 5,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label53), 0, 0.5);

  FPSSpinButton_adj = gtk_adjustment_new (1, 0, 30, 1, 10, 10);
  FPSSpinButton = gtk_spin_button_new (GTK_ADJUSTMENT (FPSSpinButton_adj), 1, 0);
  gtk_widget_show (FPSSpinButton);
  gtk_table_attach (GTK_TABLE (table8), FPSSpinButton, 3, 4, 4, 5,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);

  label52 = gtk_label_new ("Resolution");
  gtk_widget_show (label52);
  gtk_table_attach (GTK_TABLE (table8), label52, 2, 3, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label52), 0, 0.5);

  ResolutionComboBox = gtk_combo_box_new_text ();
  gtk_widget_show (ResolutionComboBox);
  gtk_table_attach (GTK_TABLE (table8), ResolutionComboBox, 3, 4, 3, 4,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (GTK_FILL), 0, 0);
  //gtk_combo_box_append_text (GTK_COMBO_BOX (ResolutionComboBox), "");

  label60 = gtk_label_new ("Driver");
  gtk_widget_show (label60);
  gtk_table_attach (GTK_TABLE (table8), label60, 0, 1, 2, 3,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_misc_set_alignment (GTK_MISC (label60), 0, 0.5);

  DriverEntry = gtk_entry_new ();
  gtk_widget_show (DriverEntry);
  gtk_table_attach (GTK_TABLE (table8), DriverEntry, 1, 4, 2, 3,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_editable_set_editable (GTK_EDITABLE (DriverEntry), FALSE);

  dialog_action_area2 = GTK_DIALOG (VideoTuningDialog)->action_area;
  gtk_widget_show (dialog_action_area2);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area2), GTK_BUTTONBOX_END);

  CancelButton = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (CancelButton);
  gtk_dialog_add_action_widget (GTK_DIALOG (VideoTuningDialog), CancelButton, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (CancelButton, GTK_CAN_DEFAULT);

  OKButton = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (OKButton);
  gtk_dialog_add_action_widget (GTK_DIALOG (VideoTuningDialog), OKButton, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (OKButton, GTK_CAN_DEFAULT);

	/* End of GUI construction */
	
	char buf[255];
	void *data[10];
	
	gtk_entry_set_text(GTK_ENTRY(DeviceNameEntry),v4l2CaputreGetName(dev));
	gtk_entry_set_text(GTK_ENTRY(LocationEntry),v4l2CaputreGetLocation(dev));
	gtk_entry_set_text(GTK_ENTRY(DriverEntry),v4l2CaptureGetDriver(dev));
	
	/* FPS */
	
	int numerator;
	int denominator;
//	int channel;
//	int norm;
//	int format;
	int i;
//	int fps;
//	int height;
//	int width;
	char *location;

	*fps = (int) round(v4l2CaptureGetFPS(dev));
			if (*fps>0){
				double min,max;
				gtk_spin_button_get_range (GTK_SPIN_BUTTON(FPSSpinButton),&min,&max);

				if (*fps > max)
					gtk_spin_button_set_range (GTK_SPIN_BUTTON(FPSSpinButton),min,*fps);
				
				gtk_spin_button_set_value(GTK_SPIN_BUTTON(FPSSpinButton),*fps);

			} else {
				gtk_widget_set_sensitive(FPSSpinButton,0);
			}
	

	char **list = v4l2CaptureGetChannelsList(dev);
				
	if (list){
		i=0;
		while (list[i]!=0){
			gtk_combo_box_append_text(GTK_COMBO_BOX(SourceComboBox),list[i]);
			i++;
		}
		
		gtk_combo_box_set_active(GTK_COMBO_BOX(SourceComboBox),0);
	}

	/* Norms List */			
	list = v4l2CaptureGetNormsList(dev);
	if (list){
		i = 0;
		while (list[i]!=0){
			gtk_combo_box_append_text(GTK_COMBO_BOX(TVNormComboBox),list[i]);
			i++;
		}
		gtk_combo_box_set_active(GTK_COMBO_BOX(TVNormComboBox),0);
	}
	/* End of Norms List */
	
	/* Format List */
	int *formats = v4l2CaptureGetImageFormatsList(dev);
	
	if (formats){
		i = 0;
		while (formats[i]!=0){
			const char *name = vidFourccToString(formats[i]);
			if (!name){
					snprintf(buf,sizeof(buf),"%08x",formats[i]);
					name = buf;								
			} 
			gtk_combo_box_append_text(GTK_COMBO_BOX(FormatComboBox),name);					
			i++;
		}
		data[0] = dev;
		data[1] = ResolutionComboBox;
		data[2] = 0;
		
		g_signal_connect(GTK_OBJECT(FormatComboBox),"changed",
			GTK_SIGNAL_FUNC(change_image_format) ,data);
			
		gtk_combo_box_set_active(GTK_COMBO_BOX(FormatComboBox),0);				
	}
				
	if (gtk_dialog_run (GTK_DIALOG (VideoTuningDialog))!= GTK_RESPONSE_OK){
			v4l2CaptureRelease(&dev);
	} else {
				if (GTK_WIDGET_SENSITIVE(FPSSpinButton)){
					*fps = (int) round(gtk_spin_button_get_value(GTK_SPIN_BUTTON(FPSSpinButton)));
					v4l2CaptureSetFPS(dev,*fps);	
				}
				
				i = gtk_combo_box_get_active(GTK_COMBO_BOX(FormatComboBox));
				if (i!=-1){
					int *f=v4l2CaptureGetImageFormatsList(dev);
					*format = f[i];
					v4l2CaptureSetImageFormat(dev,*format,0);
				}
				
				char *screen = gtk_combo_box_get_active_text(GTK_COMBO_BOX(ResolutionComboBox));
				sscanf(screen,"%dx%d",width,height);
				VidSize size;
				size.width = *width;
				size.height = *height;
				
				v4l2CaptureSetResolution(dev,&size);
				
				*channel = gtk_combo_box_get_active(GTK_COMBO_BOX(SourceComboBox));
				
				if (*channel!=-1)
					v4l2CaptureSetChannel(dev,*channel);
					
				*norm = gtk_combo_box_get_active(GTK_COMBO_BOX(TVNormComboBox));
				
				if (*norm!=-1)
					v4l2CaptureSetNorm(dev,*norm);

	}
	
	gtk_widget_destroy(VideoTuningDialog);
	
		
	return dev;	
}
/** 
 * @param location - the location of video device
 * @Return A pointer to newly created V4L2Capture structure. It must be released by v4l2CaptureRelease.
 */

V4L2Capture *video_tuning_dialog_run(const char *location){
	V4L2Capture *dev;
	int format,width,height,channel,norm,fps;
		
	dev = v4l2CaptureOpen(location);
	if (dev) {
		dev = real_video_tuning_dialog_run(dev,&format,&width,&height,
			&channel,&norm,&fps);
	}
	
	return dev;
}

/** 
 * @param location - the location of video device
 * @param format - Return the chosen image format
 * @param width - Returh the chosen image width
 * @param height - Return the chosen image height
 * @param channel - Return the chosen channel
 * @param norm - Return the chosen TVNorm
 * @param fps - Return the chosen FPS if the device is supported.    
 * @Return A pointer to newly created V4L2Capture structure. It must be released by v4l2CaptureRelease.
 */

V4L2Capture *video_tuning_dialog_run_with_props(const char *location,
		int *format,int *width,int *height,int *channel,int *norm,int *fps){
	V4L2Capture *dev;
		
	dev = v4l2CaptureOpen(location);
	if (dev) {
		dev = real_video_tuning_dialog_run(dev,format,width,height,
			channel,norm,fps);
	}
	
	return dev;
}

