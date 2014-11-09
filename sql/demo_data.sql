INSERT INTO units (un_id, un_name,un_desc) VALUES
('EA','ea.', 'each'),
('KG','kg.', 'kilogram');

INSERT INTO items (it_name, it_article, it_unit, it_type) VALUES 
('Gear', '56622100.001', 'EA', 'P'),
('Gear 2', '56622102.001', 'EA', 'P'),
('Bearing 660', '00125', 'EA', 'P'),
('Chicken egg', '', 'EA', 'M'),
('Chicken body', '', 'KG', 'M'),
('Chicken feet', '', 'KG', 'M');

INSERT INTO user_params (up_role, up_name, up_gui) VALUES ('rubus', 'Rubus admin', 
'<gui>
	<widgets>
	</widgets>
	<mainmenu>
		<menu id="" title="General">
      		<action id="Base.UserManager">Users</action>
      		<action id="Base.ReportManager">Reports manager</action>
      		<separator />
			<action id="Core.CloseSession" />
			<separator />
			<action id="MainWindow.Close" />
		</menu>
		<menu id="" title="Catalogs">
			<action id="Base.Locations">Locations</action>
			<action id="Base.Contractors">Contractors</action>
 			<action id="Base.Items">Items</action>
		</menu>
	</mainmenu>
</gui>');